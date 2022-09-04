#include "rclcpp/rclcpp.hpp"
#include "rclcpp/qos.hpp"
#include "carrl_interface/msg/curve_coeffs.hpp"
#include "mavros_msgs/srv/set_mode.hpp"
#include "mavros_msgs/srv/command_bool.hpp"
#include "mavros_msgs/msg/state.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "geometry_msgs/msg/pose_array.hpp"
#include <geometry_msgs/msg/transform_stamped.hpp>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/utils.h>
#include "Eigen/Dense" // For matrix mult.

#include <memory>
#include <chrono>
#include <functional>
#include <vector>
#include <algorithm>

#define MAX_V 1.8 // m/s

using std::placeholders::_1;
using namespace std::chrono_literals;

std::shared_ptr<rclcpp::Node> node;
std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> vel_pub;
geometry_msgs::msg::TwistStamped vel;
nav_msgs::msg::Odometry odom;
mavros_msgs::msg::State state;
carrl_interface::msg::CurveCoeffs target_curve;
tf2::Quaternion quat;
double yaw,p,r;
double max_v,car_length,x,speed_scale;
int event_horizon,action_horizon;
bool new_odom = false;

rcl_interfaces::msg::SetParametersResult param_callback(const std::vector<rclcpp::Parameter> &parameters){
  rcl_interfaces::msg::SetParametersResult result;
  result.successful = true;
  result.reason = "success";
  for(auto &p: parameters){
  RCLCPP_INFO(node->get_logger(), p.get_name().c_str());
    if(!p.get_name().compare("max_v")){
      max_v = p.as_double();
    }else if(!p.get_name().compare("car_length")){
      car_length = p.as_double();
    }else if(!p.get_name().compare("x_sample")){
      x = p.as_double();
    }else if(!p.get_name().compare("speed_scale")){
      speed_scale = p.as_double();
    }
  }
  return result;
}

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
  odom = *msg;
  //RCLCPP_INFO(node->get_logger(), "x: %.2f\ny: %.2f\nz: %.2f\n", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
  new_odom = true;
}

void state_callback(const mavros_msgs::msg::State::SharedPtr msg){
  state = *msg;
  //RCLCPP_INFO(node->get_logger(), "Connected: %d\tArmed: %d\tMode: %s",state.connected,state.armed,state.mode.c_str());
}

void curve_callback(const carrl_interface::msg::CurveCoeffs::SharedPtr msg){
  if(msg->coeffs.size() < msg->degree){
    RCLCPP_WARN(node->get_logger(), "Recieved curve has incompatible fields: degree=%d > coeffs.size()=%d",msg->degree,msg->coeffs.size());
  }else{
    target_curve = *msg;
  }
}

void timer_callback(){

}

int main(int argc, char * argv[]){
  // ROS Stuff
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("commander");
  node->declare_parameter<double>("max_v",1.0);
  node->declare_parameter<double>("car_length",0.35);
  node->declare_parameter<double>("x_sample",0.8);
  node->declare_parameter<double>("speed_scale",0.3);
  node->get_parameter("max_v", max_v);
  node->get_parameter("car_length", car_length);
  node->get_parameter("x_sample", x);
  node->get_parameter("speed_scale", speed_scale);
  auto param_cb_handle = node->add_on_set_parameters_callback(param_callback);
  rclcpp::Rate looprate(20);
  auto qos = rclcpp::QoS(1);
  qos.best_effort();
  
  //Mavros Stuff
  vel_pub = node->create_publisher<geometry_msgs::msg::TwistStamped>("/mavros/setpoint_velocity/cmd_vel", qos);
  auto set_mode_client = node->create_client<mavros_msgs::srv::SetMode>("/mavros/set_mode");
  auto arm_client = node->create_client<mavros_msgs::srv::CommandBool>("/mavros/cmd/arming");
  auto state_sub = node->create_subscription<mavros_msgs::msg::State>(
                      "/mavros/state", 1, state_callback);
  auto odom_sub = node->create_subscription<nav_msgs::msg::Odometry>(
                      "/mavros/global_position/local", qos, odom_callback);
  // For recieving path
  auto path_sub = node->create_subscription<carrl_interface::msg::CurveCoeffs>(
                      "/commander/curve", 1, curve_callback);
  // Wait for connection
  RCLCPP_INFO(node->get_logger(), "Waiting to connect...");
  std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> executor = rclcpp::executors::MultiThreadedExecutor::make_shared();
  executor->add_node(node);
  while ((!state.connected)&&rclcpp::ok()){
    looprate.sleep();
    executor->spin_some();
  }
  RCLCPP_INFO(node->get_logger(), "Connected!");
  // If we are not armed, then arm
  if(state.armed){
    RCLCPP_INFO(node->get_logger(), "Already Armed!");
  }else{
    auto arm_cmd = std::make_shared<mavros_msgs::srv::CommandBool::Request>();
    arm_cmd->value = true;

    while (!arm_client->wait_for_service(2s)) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the set_mode service. Exiting.");
        return 0;
      }
      RCLCPP_INFO(node->get_logger(), "set_mode service not available, waiting again...");
    }
    auto arm_result = arm_client->async_send_request(arm_cmd);
    // Wait for the result.
    if (executor->spin_until_future_complete(arm_result) ==
      rclcpp::FutureReturnCode::SUCCESS){
      if(arm_result.get()->success){
        RCLCPP_INFO(node->get_logger(), "ARMED");
      }else{
        RCLCPP_INFO(node->get_logger(), "ARM Failed");
      }
    }else{
      RCLCPP_ERROR(node->get_logger(), "Failed to call service /mavros/cmd/arming");
    }
  }
  //If the mode is not GUIDED, make it so
  if(state.guided){
      RCLCPP_INFO(node->get_logger(), "Already in Guided mode");
  }else{
    auto guided_mode = std::make_shared<mavros_msgs::srv::SetMode::Request>();
    guided_mode->custom_mode = "GUIDED";

    while (!set_mode_client->wait_for_service(2s)) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the set_mode service. Exiting.");
        return 0;
      }
      RCLCPP_INFO(node->get_logger(), "set_mode service not available, waiting again...");
    }
    auto set_mode_result = set_mode_client->async_send_request(guided_mode);
    // Wait for the result.
    if (executor->spin_until_future_complete(set_mode_result) ==
      rclcpp::FutureReturnCode::SUCCESS){
      if(set_mode_result.get()->mode_sent){
        RCLCPP_INFO(node->get_logger(), "Mode set to: GUIDED");
      }else{
        RCLCPP_INFO(node->get_logger(), "Mode couldn't set to: GUIDED");
      }
    }else{
      RCLCPP_ERROR(node->get_logger(), "Failed to call service /mavros/set_mode");
    }
  }
  auto timer_ = node->create_wall_timer(100ms, &timer_callback);
  RCLCPP_INFO(node->get_logger(), "Done");
  // Enter loop
  while(rclcpp::ok()){
    //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "x: %.2f\ny: %.2f\nz: %.2f\n", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
    auto message = geometry_msgs::msg::TwistStamped();
    if(new_odom){
      new_odom = false;
      if(target_curve.degree){
        double fx = 0.0;
        for(int i = 0; i < target_curve.degree; i++){
          fx += target_curve.coeffs[i]*std::pow(x,i+1);
        }
        double r = sqrt(x*x+fx*fx);
        double wz = asin(fx/r);
        double vx = std::max(0.0, std::min(max_v, wz ? r*wz/(2*x*sin(wz/2)) : r/x));
        wz = std::max(-vx/(car_length+0.125), std::min(vx/(car_length+0.125), wz/x));
        RCLCPP_INFO(node->get_logger(), "Action: %.5f, %.5f", vx,wz);
        message.twist.linear.x = vx*speed_scale;
        message.twist.angular.z = wz*speed_scale;
      }else{
        message.twist.linear.x = 0;
        message.twist.angular.z = 0;
      }
      vel_pub->publish(message);
    }
    looprate.sleep();
    executor->spin_some();
  }
  rclcpp::shutdown();
  return 0;
}