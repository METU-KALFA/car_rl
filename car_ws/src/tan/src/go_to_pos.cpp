#include "rclcpp/rclcpp.hpp"
#include "rclcpp/qos.hpp"
#include "mavros_msgs/srv/set_mode.hpp"
#include "mavros_msgs/srv/command_bool.hpp"
#include "mavros_msgs/msg/state.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

#include <memory>
#include <chrono>
#include <functional>
#include <vector>

using std::placeholders::_1;
using namespace std::chrono_literals;

uint8_t pose_idx = 0;
std::vector<geometry_msgs::msg::Point> poses;
nav_msgs::msg::Odometry odom;
mavros_msgs::msg::State state;

std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::PoseStamped>> pose_pub;

void add_point(double x,double y,double z){
  geometry_msgs::msg::Point point;
  point.x = x;
  point.y = y;
  point.z = z;
  poses.emplace_back(point);
}

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
  odom = *msg;
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "x: %.2f\ny: %.2f\nz: %.2f\n", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
}

void state_callback(const mavros_msgs::msg::State::SharedPtr msg){
  state = *msg;
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Connected: %d\tArmed: %d\tMode: %s",state.connected,state.armed,state.mode.c_str());
}

void timer_callback(){
  auto message = geometry_msgs::msg::PoseStamped();
  auto &pose = poses[pose_idx++];
  if (pose_idx==poses.size()) pose_idx = 0;
  message.pose.position = pose;
  pose_pub->publish(message);
}

int main(int argc, char * argv[]){
  rclcpp::init(argc, argv);
  poses.reserve(10);
  add_point(0,0,0);
  add_point(3,0,0);
  add_point(3,3,0);
  add_point(0,3,0);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("pose_pub");
  rclcpp::Rate looprate(20);
  auto qos = rclcpp::QoS(1);
  qos.best_effort();

  pose_pub = node->create_publisher<geometry_msgs::msg::PoseStamped>("/mavros/setpoint_position/local", 10);
      
  auto set_mode_client = node->create_client<mavros_msgs::srv::SetMode>("/mavros/set_mode");
  auto arm_client = node->create_client<mavros_msgs::srv::CommandBool>("/mavros/cmd/arming");
  

  auto state_sub =  node->create_subscription<mavros_msgs::msg::State>(
                      "/mavros/state", 1, state_callback);
  auto odom_sub =  node->create_subscription<nav_msgs::msg::Odometry>(
                      "/mavros/global_position/local", qos, odom_callback);

  // Wait for connection
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Waiting to connect... \\");
  std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> executor = rclcpp::executors::MultiThreadedExecutor::make_shared();
  executor->add_node(node);
  while ((!state.connected)&&rclcpp::ok()){
    looprate.sleep();
    executor->spin_some();
  }
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Connected!");
  // If we are not armed, then arm
  if(state.armed){
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Already Armed!");
  }else{
    auto arm_cmd = std::make_shared<mavros_msgs::srv::CommandBool::Request>();
    arm_cmd->value = true;

    while (!arm_client->wait_for_service(2s)) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the set_mode service. Exiting.");
        return 0;
      }
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "set_mode service not available, waiting again...");
    }
    auto arm_result = arm_client->async_send_request(arm_cmd);
    // Wait for the result.
    if (executor->spin_until_future_complete(arm_result) ==
      rclcpp::FutureReturnCode::SUCCESS){
      if(arm_result.get()->success){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "ARMED");
      }else{
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "ARM Failed");
      }
    }else{
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service /mavros/cmd/arming");
    }
  }
  //If the mode is not GUIDED, make it so
  if(state.guided){
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Already in Guided mode");
  }else{
    auto guided_mode = std::make_shared<mavros_msgs::srv::SetMode::Request>();
    guided_mode->custom_mode = "GUIDED";

    while (!set_mode_client->wait_for_service(2s)) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the set_mode service. Exiting.");
        return 0;
      }
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "set_mode service not available, waiting again...");
    }
    auto set_mode_result = set_mode_client->async_send_request(guided_mode);
    // Wait for the result.
    if (executor->spin_until_future_complete(set_mode_result) ==
      rclcpp::FutureReturnCode::SUCCESS){
      if(set_mode_result.get()->mode_sent){
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Mode set to: GUIDED");
      }else{
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Mode couldn't set to: GUIDED");
      }
    }else{
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service /mavros/set_mode");
    }
  }
  auto timer_ = node->create_wall_timer(5s, &timer_callback);
  RCLCPP_INFO(node->get_logger(), "Done");
  // Enter loop
  /*while(rclcpp::ok()){
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "x: %.2f\ny: %.2f\nz: %.2f\n", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
    looprate.sleep();
    executor->spin_once();
  }*/
  executor->spin();
  rclcpp::shutdown();
  return 0;
}