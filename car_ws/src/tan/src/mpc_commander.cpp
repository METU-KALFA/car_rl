#include "rclcpp/rclcpp.hpp"
#include "rclcpp/qos.hpp"
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
tf2::Quaternion quat;
Eigen::MatrixX2d actionSet;
double yaw,p,r;
double max_v,car_length;
int event_horizon,action_horizon;

std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> vel_pub;

inline Eigen::MatrixX2d msg2matrix(const geometry_msgs::msg::PoseArray &msg){
  Eigen::MatrixX2d m(event_horizon,2);
  for(unsigned int i = 0; i < (unsigned) event_horizon; i++){
    if(i < msg.poses.size()){
      m(i,0) = msg.poses[i].position.x;
      m(i,1) = msg.poses[i].position.y;
    }else{
      m.block<1,2>(i,0) = m.block<1,2>(i-1,0);
    }
  }
  return m;
}

inline Eigen::RowVector3d translation2d(const Eigen::Ref<const Eigen::RowVector3d> &s, const double &vx, const double &wz, const double &dt = 0.1){
  double dr = vx*cos(wz*dt/(2*1.735))*dt; // Approx. of 2*vx*sin(wz*dt/2)/wz, good for -10<wz<10 (to aviod zero div. when wz=0)
  return s + Eigen::RowVector3d{dr*cos(s(2)+wz*dt), dr*sin(s(2)+wz*dt), wz*dt};
}

inline Eigen::MatrixX2d model_iterate(const Eigen::Ref<const Eigen::RowVector3d> &init, const Eigen::Ref<const Eigen::MatrixX2d> &actionSet){
  Eigen::MatrixX2d points(event_horizon,2);
  Eigen::RowVector3d next = init;
  for(int i = 0; i < event_horizon; i++){
    next = translation2d(next, actionSet(std::min(i,((int)actionSet.rows())-1),0),actionSet(std::min(i,((int)actionSet.rows())-1),1));
    points.block<1,2>(i,0) = next.block<1,2>(0,0);
  }
  return points;
}

inline double calc_path_cost(const Eigen::Ref<const Eigen::MatrixX2d> &p1, const Eigen::Ref<const Eigen::MatrixX2d> &p2){
  return (p1 - p2).norm();
}

double mpc(double &x, double &y, double &yaw, const Eigen::Ref<const Eigen::MatrixX2d> &dest, Eigen::Ref<Eigen::MatrixX2d> opActionSet){
  Eigen::RowVector3d current{x, y, yaw};
  Eigen::RowVector2d step{0.1, 0.1}; // d_vx, d_wz
  double pastCost(0.0), cost(0.0);
  for(int k = 0; k < 2; k++){ // Re-iteration, Should be carefull about resetting cost and pastCost to 0
    RCLCPP_INFO(node->get_logger(), "k: %d",k);
    for(int i = 0; i < action_horizon; i++){
      RCLCPP_INFO(node->get_logger(), "i: %d",i);
      //pastCost = 0.0; 
      for(int t = 0; t < 10; t++){
        cost -= 0.1;
        for(int j = 0; j < 10; j++){
          opActionSet(i,t%2) += (pastCost - cost)*step(t%2);
          if((pastCost - cost) < 0) step *= -1;
          opActionSet(i,0) = opActionSet(i,0) < 0 ? 0 : opActionSet(i,0); // Hard-constraint (only forward speed)
          opActionSet(i,1) = std::min(opActionSet(i,0)/car_length,std::max(-opActionSet(i,0)/car_length,opActionSet(i,1))); // Hard-constraint (max 45 degree turning)
          pastCost = cost;
          cost = calc_path_cost(model_iterate(current, opActionSet),dest);
          if(t%2){
            if(i){
              cost += 0.7*std::abs(opActionSet(i,1)-opActionSet(i-1,1));
            }
          }else{
            cost += 10*(std::max(max_v,opActionSet(i,0)) - max_v); // Soft-constraint
            cost += 5*(0.4 - std::min(0.4,opActionSet(i,0))); // Soft-constraint
          }
          RCLCPP_INFO(node->get_logger(), "Cost: %.7f", cost);
        }
      }
    }
  }
  return cost;
}

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
  odom = *msg;
  //RCLCPP_INFO(node->get_logger(), "x: %.2f\ny: %.2f\nz: %.2f\n", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
  auto message = geometry_msgs::msg::TwistStamped();
  if(path.poses.size()){
    tf2::fromMsg(odom.pose.pose.orientation, quat);
    tf2::Matrix3x3 matrix(quat);
    matrix.getEulerYPR(yaw,p,r);
    Eigen::MatrixX2d destPath = msg2matrix(path);
    actionSet.block(0,0,action_horizon-1,2) = actionSet.block(1,0,action_horizon-1,2);
    mpc(odom.pose.pose.position.x, odom.pose.pose.position.y, yaw, destPath, actionSet);
    RCLCPP_INFO(node->get_logger(), "Action: %.7f, %.7f", actionSet(0,0),actionSet(0,1));
    message.twist.linear.x = actionSet(0,0);
    message.twist.angular.z = actionSet(0,1);
  }
  vel_pub->publish(message);
}

void state_callback(const mavros_msgs::msg::State::SharedPtr msg){
  state = *msg;
  //RCLCPP_INFO(node->get_logger(), "Connected: %d\tArmed: %d\tMode: %s",state.connected,state.armed,state.mode.c_str());
}

void path_callback(const geometry_msgs::msg::PoseArray::SharedPtr msg){
  path = *msg;
}


void timer_callback(){

}

int main(int argc, char * argv[]){
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("commander");
  node->declare_parameter<double>("max_v",1.0);
  node->declare_parameter<double>("car_length",0.35);
  node->declare_parameter<int>("event_horizon",30);
  node->declare_parameter<int>("action_horizon",6);
  node->get_parameter("max_v", max_v);
  node->get_parameter("car_length", car_length);
  node->get_parameter("event_horizon", event_horizon);
  node->get_parameter("action_horizon", action_horizon);
  rclcpp::Rate looprate(20);
  auto qos = rclcpp::QoS(1);
  qos.best_effort();
  actionSet = Eigen::MatrixX2d(action_horizon,2);
  actionSet << Eigen::VectorXd::Constant(action_horizon, max_v/2), Eigen::VectorXd::Zero(action_horizon);

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