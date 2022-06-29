#include <cstdio>
#include "service_interface/srv/location.hpp"
#include <chrono>
#include <rclcpp/rclcpp.hpp>
using namespace std::chrono_literals;
void location_get(rclcpp::Client<service_interface::srv::Location>::SharedFuture &f, double &x, double &y, double &yaw)
{
  auto status = f.wait_for(3s); // not spinning here!
  if (status == std::future_status::ready)
  {
    auto location_response = f.get();
    x = location_response->x;
    y = location_response->y;
    yaw = location_response->yaw;
  }
}
struct simple_test_client : public rclcpp::Node
{
  simple_test_client() : rclcpp::Node("simple")
  {
    for (size_t i = 0; i < 2; i++)
    {
      callback_groups[i] = this->create_callback_group(rclcpp::callback_group::CallbackGroupType::MutuallyExclusive);
    }
    timer = this->create_wall_timer(5ms, std::bind(&simple_test_client::timer_callback, this), callback_groups[0]);
    location_client = this->create_client<service_interface::srv::Location>("/service/odom", rmw_qos_profile_services_default, callback_groups[1]);
  }
  rclcpp::TimerBase::SharedPtr timer;
  rclcpp::Client<service_interface::srv::Location>::SharedPtr location_client;
  rclcpp::CallbackGroup::SharedPtr callback_groups[2];
  void timer_callback()
  {
    RCLCPP_INFO(this->get_logger(), "Timer callback");

    rclcpp::Client<service_interface::srv::Location>::SharedRequest client_request = std::make_shared<service_interface::srv::Location::Request>();
    auto location_srv = location_client->async_send_request(client_request);
    double x, y, yaw;
    location_get(location_srv, x, y, yaw);
    RCLCPP_INFO(this->get_logger(), "x: %lf ,y: %lf ,yaw: %lf", x, y, yaw);
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor;
  auto node = std::make_shared<simple_test_client>();
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
