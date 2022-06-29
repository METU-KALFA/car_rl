#include "service_calls.hpp"

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
void force_get(rclcpp::Client<service_interface::srv::Force>::SharedFuture &f, double &x, double &y)
{
  auto status = f.wait_for(3s); // not spinning here!
  if (status == std::future_status::ready)
  {
    auto force_response = f.get();
    x = force_response->x;
    y = force_response->y;
  }
}
void ready_get(rclcpp::Client<service_interface::srv::Ready>::SharedFuture &f, bool &ready)
{
  auto status = f.wait_for(3s); // not spinning here!
  if (status == std::future_status::ready)
  {
    auto ready_response = f.get();
    ready = ready_response->ready;
  }
}
void force_boundary_get(rclcpp::Client<service_interface::srv::ForceBoundaryCheck>::SharedFuture &f, double &x, double &y, bool &is_overwritten)
{
  auto status = f.wait_for(3s); // not spinning here!
  if (status == std::future_status::ready)
  {
    auto fb_response = f.get();
    x = fb_response->x;
    y = fb_response->y;
    is_overwritten = fb_response->is_overwritten;
  }
}
void velocity_boundary_get(rclcpp::Client<service_interface::srv::VelocityBoundaryCheck>::SharedFuture &f, double &x, double &y)
{
  auto status = f.wait_for(3s); // not spinning here!
  if (status == std::future_status::ready)
  {
    auto vb_response = f.get();
    x = vb_response->x;
    y = vb_response->y;
  }
}

