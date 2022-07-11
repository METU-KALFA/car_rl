#include <cstdio>
#include "service_interface/srv/location.hpp"
#include "service_interface/srv/force.hpp"
#include "service_interface/srv/ready.hpp"
#include <rclcpp/rclcpp.hpp>
#include "std_msgs/msg/empty.hpp"
#include "service_client_lib/service_calls.hpp"
#include <semaphore.h>
#include <cmath>
sem_t message_wait;
// bad design elevate to parameter serve
float goal_x = 70.77;
float goal_y = -3.79;

struct goal : public rclcpp::Node
{
  goal() : Node("goal")
  {
    sem_init(&message_wait, 0, 0);
    timer_callback_group = create_callback_group(rclcpp::callback_group::CallbackGroupType::MutuallyExclusive);
    ready_callback_group = create_callback_group(rclcpp::callback_group::CallbackGroupType::MutuallyExclusive);
    force_callback_group = create_callback_group(rclcpp::callback_group::CallbackGroupType::MutuallyExclusive);
    location_callback_group = create_callback_group(rclcpp::callback_group::CallbackGroupType::MutuallyExclusive);
    location_client = this->create_client<service_interface::srv::Location>("/service/odom", rmw_qos_profile_services_default, location_callback_group);
    i_location_client = this->create_client<service_interface::srv::Location>("/init/odom", rmw_qos_profile_services_default, location_callback_group);
    timer = this->create_wall_timer(std::chrono::milliseconds(200), std::bind(&goal::timer_callback, this), timer_callback_group);
    ready_service = this->create_service<service_interface::srv::Ready>("/ready/goal", std::bind(&goal::ready_callback, this, std::placeholders::_1, std::placeholders::_2), rmw_qos_profile_services_default, ready_callback_group);
    force_service = this->create_service<service_interface::srv::Force>("/force/goal", std::bind(&goal::force_callback, this, std::placeholders::_1, std::placeholders::_2), rmw_qos_profile_services_default, force_callback_group);
    shutdown_pub = this->create_publisher<std_msgs::msg::Empty>("/shutdown", 10);
    location_client->wait_for_service();
    i_location_client->wait_for_service();
  }
  bool is_ready = false;
  float force_x = 0.0;
  float force_y = 0.0;
  float send_x = 0.0;
  float send_y = 0.0;
  std::mutex mtx;
  rclcpp::TimerBase::SharedPtr timer;
  rclcpp::Client<service_interface::srv::Location>::SharedPtr location_client;
  rclcpp::Client<service_interface::srv::Location>::SharedPtr i_location_client;
  rclcpp::Service<service_interface::srv::Ready>::SharedPtr ready_service;
  rclcpp::Service<service_interface::srv::Force>::SharedPtr force_service;
  rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr shutdown_pub;
  rclcpp::CallbackGroup::SharedPtr timer_callback_group;
  rclcpp::CallbackGroup::SharedPtr ready_callback_group;
  rclcpp::CallbackGroup::SharedPtr force_callback_group;
  rclcpp::CallbackGroup::SharedPtr location_callback_group;
  void first_time()
  {
    double x, y, yaw;
    // The following three lines are a good display of how to use the service calls.
    rclcpp::Client<service_interface::srv::Location>::SharedRequest client_request = std::make_shared<service_interface::srv::Location::Request>();
    auto location_srv = i_location_client->async_send_request(client_request); // initial location is requested
    location_get(location_srv, x, y, yaw);
    // The three lines end here.
    goal_x = goal_x + x; // we want a displacement from current point rather than an absolute point.
    goal_y = goal_y + y; 
  }
  void timer_callback()
  {
    static bool first = true;
    if (first)
    {
      first_time();
      first = false;
    }
    rclcpp::Client<service_interface::srv::Location>::SharedRequest client_request = std::make_shared<service_interface::srv::Location::Request>();
    double x, y, yaw;
    auto location_srv = i_location_client->async_send_request(client_request);
    location_get(location_srv, x, y, yaw);

    float displacement_x = goal_x - x;
    float displacement_y = goal_y - y;

    if (abs(displacement_x) <= 1 && abs(displacement_y) <= 1)
    {
      // Arrival to the goal position: 
      // Shutdown message is sent, and is consumed by the pixhawk node
      auto msg = std_msgs::msg::Empty();
      shutdown_pub->publish(msg);
      return;
    }
    double f_absolute,f_x_local_coordinate,f_y_local_coordinate;
    double alpha;//angle between force vector and the east vector
    alpha = atan2(displacement_y,displacement_x);
    f_absolute =  displacement_x * displacement_x+ displacement_y * displacement_y;
    f_x_local_coordinate = f_absolute * sin(alpha+yaw);
    f_y_local_coordinate = f_absolute * cos(alpha+yaw);
    // All variables that are interfacing with a ros2 service must be updated in a critical section to
    // not be gathered in between the update and the request to recieve them. In other words we do not want 
    // force_x be a newly updated value and force_y be an old value when the ready request tries to gather the values.
    mtx.lock();
    force_x = f_x_local_coordinate;
    force_y = f_y_local_coordinate;
    is_ready = true;
    mtx.unlock();
  }
  void ready_callback(const std::shared_ptr<service_interface::srv::Ready::Request> request, std::shared_ptr<service_interface::srv::Ready::Response> response)
  {
    std::lock_guard<std::mutex> lock(mtx);
    response->ready = is_ready;
    send_x = force_x;
    send_y = force_y;
  }
  void force_callback(const std::shared_ptr<service_interface::srv::Force::Request> request, std::shared_ptr<service_interface::srv::Force::Response> response)
  {
    std::lock_guard<std::mutex> lock(mtx);
    // RCLCPP_INFO(this->get_logger(), "GOAL FORCE GOT REQUESTED\n");
    response->x = send_x;
    response->y = send_y;
    is_ready = false;
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor;
  auto node = std::make_shared<goal>();
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
