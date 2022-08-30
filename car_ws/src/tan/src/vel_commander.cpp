#include "rclcpp/rclcpp.hpp"
#include "rclcpp/qos.hpp"
#include "mavros_msgs/srv/set_mode.hpp"
#include "mavros_msgs/srv/command_bool.hpp"
#include "mavros_msgs/msg/state.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "geometry_msgs/msg/pose_array.hpp"

#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/utils.h>

#include <memory>
#include <chrono>
#include <functional>
#include <vector>

#define KP_R 1
#define KP_H 0.3
#define KI_R 0
#define KI_H 0
#define KD_R 0 //1
#define KD_H 0 //1

using std::placeholders::_1;
using namespace std::chrono_literals;

std::shared_ptr<rclcpp::Node> node;
geometry_msgs::msg::TwistStamped vel;
nav_msgs::msg::Odometry odom;
mavros_msgs::msg::State state;
geometry_msgs::msg::PoseArray path;

std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> vel_pub;

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
  odom = *msg;
  RCLCPP_INFO(node->get_logger(), "x: %.2f\ny: %.2f\nz: %.2f\n", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
}

void state_callback(const mavros_msgs::msg::State::SharedPtr msg){
  state = *msg;
  RCLCPP_INFO(node->get_logger(), "Connected: %d\tArmed: %d\tMode: %s",state.connected,state.armed,state.mode.c_str());
}

void path_callback(const geometry_msgs::msg::PoseArray::SharedPtr msg){
  path = *msg;
}

double err_r,err_h,last_err_r,last_err_h;
double last_time;
double dt,i_r,i_h;
tf2::Quaternion quat;

void timer_callback(){
  auto message = geometry_msgs::msg::TwistStamped();
  auto point = path.poses[0].position;
  double yaw,p,r;
  tf2::fromMsg(odom.pose.pose.orientation, quat);
  tf2::Matrix3x3 matrix(quat);
  matrix.getEulerYPR(yaw,p,r);
  //RCLCPP_INFO(node->get_logger(), "Yaw: %.2f, Dest. Yaw: %.2f", yaw,atan2(point.y - odom.pose.pose.position.y,point.x - odom.pose.pose.position.x));

  err_r = sqrt((point.x - odom.pose.pose.position.x)*(point.x - odom.pose.pose.position.x)+(point.y - odom.pose.pose.position.y)*(point.y - odom.pose.pose.position.y));
  err_h = atan2(point.y - odom.pose.pose.position.y,point.x - odom.pose.pose.position.x) - yaw;
  err_h = err_h > M_PI ? err_h - 2*M_PI: err_h;
  err_h = err_h <-M_PI ? err_h + 2*M_PI: err_h;

  if(last_err_r*err_r<0){ // Anti-windup
      i_r = 0;
  }
  if(last_err_h*err_h<0){ // Anti-windup
      i_h = 0;
  }

  dt = node->now().seconds() - last_time;
  i_r += err_r*dt;
  i_h += err_h*dt;
  
  if(err_r>3){
    message.twist.linear.x = KP_R*err_r + KD_R*(err_r - last_err_r)/dt + KI_R*i_r;
    message.twist.angular.z = KP_H*err_h + KD_H*(err_h - last_err_h)/dt + KI_H*i_h;
  }else{
    message.twist.linear.x = 0;
    message.twist.angular.z = 0;
  }

  last_time = node->now().seconds();
  last_err_r = err_r;
  last_err_h = err_h;

  vel_pub->publish(message);
}

int main(int argc, char * argv[]){
  rclcpp::init(argc, argv);
  path.poses.push_back(geometry_msgs::msg::Pose());

  node = rclcpp::Node::make_shared("commander");
  rclcpp::Rate looprate(20);
  auto qos = rclcpp::QoS(1);
  qos.best_effort();
  //for mavros
  vel_pub = node->create_publisher<geometry_msgs::msg::TwistStamped>("/mavros/setpoint_velocity/cmd_vel", qos);
      
  auto set_mode_client = node->create_client<mavros_msgs::srv::SetMode>("/mavros/set_mode");
  auto arm_client = node->create_client<mavros_msgs::srv::CommandBool>("/mavros/cmd/arming");

  auto state_sub =  node->create_subscription<mavros_msgs::msg::State>(
                      "/mavros/state", 1, state_callback);
  auto odom_sub =  node->create_subscription<nav_msgs::msg::Odometry>(
                      "/mavros/global_position/local", qos, odom_callback);
  //for recieving path
  auto path_sub =  node->create_subscription<geometry_msgs::msg::PoseArray>(
                      "/commander/path", 1, path_callback);
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
  last_time = node->now().seconds();
  auto timer_ = node->create_wall_timer(100ms, &timer_callback);
  RCLCPP_INFO(node->get_logger(), "Done");
  // Enter loop
  /*while(rclcpp::ok()){
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "x: %.2f\ny: %.2f\nz: %.2f\n", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
    looprate.sleep();
    executor->spin_some();
  }*/
  executor->spin();
  rclcpp::shutdown();
  return 0;
}