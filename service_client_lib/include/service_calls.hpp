#include <rclcpp/rclcpp.hpp>
#include "service_interface/srv/location.hpp"

void location_get(rclcpp::Client<service_interface::srv::Location>::SharedFuture &f, double &x, double &y, double &yaw);
void force_get(rclcpp::Client<service_interface::srv::Force>::SharedFuture &f, double &x, double &y);
void ready_get(rclcpp::Client<service_interface::srv::Ready>::SharedFuture &f, bool &ready);
void force_boundary_get(rclcpp::Client<service_interface::srv::ForceBoundaryCheck>::SharedFuture &f, double &x, double &y, bool &is_overwritten);
void velocity_boundary_get(rclcpp::Client<service_interface::srv::VelocityBoundaryCheck>::SharedFuture &f, double &x, double &y);