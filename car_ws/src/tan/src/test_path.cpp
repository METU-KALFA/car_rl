#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_array.hpp"
#include "mavros_msgs/msg/state.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <vector>
#include <memory>
#include <chrono>

using namespace std::chrono_literals;

std::shared_ptr<rclcpp::Node> node;
std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::PoseArray>> path_pub;
geometry_msgs::msg::PoseArray msg;
nav_msgs::msg::Odometry odom;
mavros_msgs::msg::State state;
std::vector<geometry_msgs::msg::Pose> poses;
int pose_idx = 0;
int event_horizon = 20;

void add_point(double x,double y,double z){
  geometry_msgs::msg::Pose point;
  point.position.x = x;
  point.position.y = y;
  point.position.z = z;
  poses.emplace_back(point);
}

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
  odom = *msg;
}

void state_callback(const mavros_msgs::msg::State::SharedPtr msg){
  state = *msg;
}

void timer_callback(){
  double r = sqrt(pow((odom.pose.pose.position.x - msg.poses[0].position.x),2) + pow((odom.pose.pose.position.y - msg.poses[0].position.y),2));
  RCLCPP_INFO(node->get_logger(), "r: %.2f",r);
  if(r < 1.2){
    RCLCPP_INFO(node->get_logger(), "current WP: %.2f, %.2f car: %.2f, %.2f", msg.poses[0].position.x, msg.poses[0].position.y, odom.pose.pose.position.x, odom.pose.pose.position.y);
    msg.poses.erase(msg.poses.begin());
    msg.poses.push_back(poses[pose_idx++]);
    RCLCPP_INFO(node->get_logger(), "Next: %.2f, %.2f r: %.2f", msg.poses[0].position.x, msg.poses[0].position.y, r);
    path_pub->publish(msg);
  }
}

int main(int argc, char * argv[]){
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("path_sender");
  rclcpp::Rate looprate(20);
  auto qos = rclcpp::QoS(1);
  qos.best_effort();
  msg.poses.reserve(event_horizon);
  poses.reserve(100);
  for(int i = 0; i < 100; i++){
    add_point(17*(cos(2*M_PI*i/100)-1),17*sin(2*M_PI*i/100),0);
  }
  // for(int i = 0; i < 100; i++){
  //   add_point(0,i*1.2,0);
  // }
  for(pose_idx = 0; pose_idx < event_horizon; pose_idx++){
    msg.poses.push_back(poses[pose_idx]);
  }
  //for mavros
  auto state_sub =  node->create_subscription<mavros_msgs::msg::State>(
                      "/mavros/state", 1, state_callback);
  auto odom_sub =  node->create_subscription<nav_msgs::msg::Odometry>(
                      "/mavros/global_position/local", qos, odom_callback);
  path_pub = node->create_publisher<geometry_msgs::msg::PoseArray>("/commander/path", 10);

  while ((!state.connected)&&rclcpp::ok()){
    looprate.sleep();
    rclcpp::spin_some(node);
  }
  RCLCPP_INFO(node->get_logger(), "Connected!");
  path_pub->publish(msg);
  auto timer_ = node->create_wall_timer(100ms, &timer_callback);
  RCLCPP_INFO(node->get_logger(), "Done");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
    