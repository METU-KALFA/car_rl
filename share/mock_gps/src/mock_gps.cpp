#include <cstdio>
#include "service_interface/srv/location.hpp"
#include "msg_interface/msg/control.hpp"
#include "std_msgs/msg/empty.hpp"
#include <rclcpp/rclcpp.hpp>
#include <mutex>
double position_x =0;
double position_y =0;
double position_yaw =0;
std::mutex read_location_msg_mtx;
struct mock_gps : rclcpp::Node
{
  mock_gps() : rclcpp::Node("mock_gps")
  {
    cg1 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    cg2 = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

    auto opt = rclcpp::SubscriptionOptions();
    opt.callback_group = cg1;
    location_service = this->create_service<service_interface::srv::Location>("/service/odom", std::bind(&mock_gps::location_callback, this, std::placeholders::_1, std::placeholders::_2),rmw_qos_profile_services_default, cg2);
    i_location_service = this->create_service<service_interface::srv::Location>("/init/odom", std::bind(&mock_gps::i_location_callback, this, std::placeholders::_1, std::placeholders::_2),rmw_qos_profile_services_default,cg2);
    control_sub = this->create_subscription<msg_interface::msg::Control>("/control",10, std::bind(&mock_gps::control_callback, this, std::placeholders::_1), opt);
    shutdown_sub = this->create_subscription<std_msgs::msg::Empty>("/shutdown",10, std::bind(&mock_gps::shutdown_callback, this, std::placeholders::_1), opt);
	timer = this->create_wall_timer(std::chrono::milliseconds(10), std::bind(&mock_gps::timer_callback, this));
  }
  rclcpp::Service<service_interface::srv::Location>::SharedPtr location_service;
  rclcpp::Service<service_interface::srv::Location>::SharedPtr i_location_service;
  rclcpp::Subscription<msg_interface::msg::Control>::SharedPtr control_sub;
  rclcpp::Subscription<std_msgs::msg::Empty>::SharedPtr shutdown_sub;
  rclcpp::TimerBase::SharedPtr timer;
  rclcpp::CallbackGroup::SharedPtr cg1;
  rclcpp::CallbackGroup::SharedPtr cg2;
  void location_callback(const std::shared_ptr<service_interface::srv::Location::Request> request,
                          std::shared_ptr<service_interface::srv::Location::Response> response)
  { 
    read_location_msg_mtx.lock();
    response->x = position_x;
    response->y = position_y;
    response->yaw = position_yaw;
    read_location_msg_mtx.unlock();
  }
  void i_location_callback(const std::shared_ptr<service_interface::srv::Location::Request> request,
                          std::shared_ptr<service_interface::srv::Location::Response> response)
  {
    response->x = 0;
    response->y = 0;
    response->yaw = 0;
  }
  void timer_callback()
  {
  }
  void shutdown_callback(const std_msgs::msg::Empty::SharedPtr msg)
  {
	  rclcpp::shutdown();
  }

  void control_callback(const msg_interface::msg::Control::SharedPtr msg)
  {
     read_location_msg_mtx.lock();
    position_x += msg->vx*0.1;
    position_y += msg->vy*0.1;
     read_location_msg_mtx.unlock();
    return;
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor;
  auto node = std::make_shared<mock_gps>();
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}

