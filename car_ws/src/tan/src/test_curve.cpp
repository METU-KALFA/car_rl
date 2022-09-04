#include "rclcpp/rclcpp.hpp"
#include "carrl_interface/msg/curve_coeffs.hpp"
#include "mavros_msgs/msg/state.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <vector>
#include <memory>
#include <chrono>

using namespace std::chrono_literals;

std::shared_ptr<rclcpp::Node> node;
std::shared_ptr<rclcpp::Publisher<carrl_interface::msg::CurveCoeffs>> curve_pub;
carrl_interface::msg::CurveCoeffs curve;
nav_msgs::msg::Odometry odom;
mavros_msgs::msg::State state;
std::vector<geometry_msgs::msg::Pose> poses;
int pose_idx = 0;

rcl_interfaces::msg::SetParametersResult param_callback(const std::vector<rclcpp::Parameter> &parameters){
  rcl_interfaces::msg::SetParametersResult result;
  result.successful = true;
  result.reason = "success";
  for(auto &p: parameters){
    if(!p.get_name().compare("degree")){
      curve.degree = p.as_int();
    }else if(p.get_name()[0] == 'c'){
      int index = stoi(p.get_name().substr(1))-1;
      if(index < (int)curve.coeffs.size()){
        curve.coeffs[index] = p.as_double();
      }else{
        result.successful = false;
        char a[100];
        sprintf(a, "Param %s requires param /curve_sender/degree to be >= %d", p.get_name().c_str(),index + 1);
        result.reason = a;
      }
    }
  }
  return result;
}

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
  odom = *msg;
}

void state_callback(const mavros_msgs::msg::State::SharedPtr msg){
  state = *msg;
}

void timer_callback(){
  RCLCPP_INFO(node->get_logger(), "car: %.2f, %.2f", odom.pose.pose.position.x, odom.pose.pose.position.y);
  node->get_parameter("degree", curve.degree);
  node->get_parameter("c1", curve.coeffs[0]);
  node->get_parameter("c2", curve.coeffs[1]);
  node->get_parameter("c3", curve.coeffs[2]);
  curve_pub->publish(curve);
}

int main(int argc, char * argv[]){
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("curve_sender");
  rclcpp::Rate looprate(20);
  auto qos = rclcpp::QoS(1);
  qos.best_effort();
  curve.coeffs.emplace_back(0);
  curve.coeffs.emplace_back(0);
  node->declare_parameter<int>("degree",2);
  node->declare_parameter<double>("c1",0.0);
  node->declare_parameter<double>("c2",0.2);
  node->get_parameter("degree", curve.degree);
  node->get_parameter("c1", curve.coeffs[0]);
  node->get_parameter("c2", curve.coeffs[1]);
  auto param_cb_handle = node->add_on_set_parameters_callback(param_callback);
  
  //for mavros
  auto state_sub =  node->create_subscription<mavros_msgs::msg::State>(
                      "/mavros/state", 1, state_callback);
  auto odom_sub =  node->create_subscription<nav_msgs::msg::Odometry>(
                      "/mavros/global_position/local", qos, odom_callback);
  curve_pub = node->create_publisher<carrl_interface::msg::CurveCoeffs>("/commander/curve", 1);

  while ((!state.connected)&&rclcpp::ok()){
    looprate.sleep();
    rclcpp::spin_some(node);
  }
  RCLCPP_INFO(node->get_logger(), "Connected!");
  auto timer_ = node->create_wall_timer(500ms, &timer_callback);
  RCLCPP_INFO(node->get_logger(), "Done");
  while(rclcpp::ok()){
    while((int)curve.coeffs.size() < curve.degree){
      curve.coeffs.emplace_back(0);
      char a[5];
      sprintf(a,"c%ld",curve.coeffs.size());
      node->declare_parameter<double>(a,0.0);
    }
    rclcpp::spin_some(node);
    looprate.sleep();
  }
  rclcpp::shutdown();
  return 0;
}
    