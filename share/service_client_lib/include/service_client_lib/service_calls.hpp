#include <rclcpp/rclcpp.hpp>
#include "service_interface/srv/location.hpp"
#include "service_interface/srv/force.hpp"
#include "service_interface/srv/ready.hpp"
#include "service_interface/srv/force_boundary_check.hpp"
#include "service_interface/srv/velocity_boundary_check.hpp"
#include <chrono>
// @KEY: Every service interface needs to get a get function implemented here do not forget to include it in the headers.
void location_get(rclcpp::Client<service_interface::srv::Location>::SharedFuture &f, double &x, double &y, double &yaw);
void force_get(rclcpp::Client<service_interface::srv::Force>::SharedFuture &f, double &x, double &y);
void ready_get(rclcpp::Client<service_interface::srv::Ready>::SharedFuture &f, bool &ready);
void force_boundary_get(rclcpp::Client<service_interface::srv::ForceBoundaryCheck>::SharedFuture &f, double &x, double &y, bool &is_overwritten);
void velocity_boundary_get(rclcpp::Client<service_interface::srv::VelocityBoundaryCheck>::SharedFuture &f, double &x, double &y);