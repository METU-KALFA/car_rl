#include "service_calls.hpp"
using namespace std::chrono_literals;
/*
How to write a get function:
name_get(SharedFuture& f, out_type1& out1,out_type2& out2, ...)
wait for 1ms
if status is ready
****use get call over the shared future
****write all the fields of response to out parameters of your function (the reference parameters after the future like out1 out2 ... outn)
else
**** wait for 1ms, and check status again.
*/
void location_get(rclcpp::Client<service_interface::srv::Location>::SharedFuture &f, double &x, double &y, double &yaw)
{
  auto status = f.wait_for(1ms); // not spinning here!
  check_again:
  if (status == std::future_status::ready)
  {
    auto location_response = f.get();
    x = location_response->x;
    y = location_response->y;
    yaw = location_response->yaw;
  }
  else{
    status = f.wait_for(1ms);
    goto check_again;
  }
}
void force_get(rclcpp::Client<service_interface::srv::Force>::SharedFuture &f, double &x, double &y)
{
  auto status = f.wait_for(1ms); // not spinning here!
  check_again:
  if (status == std::future_status::ready)
  {
    auto force_response = f.get();
    x = force_response->x;
    y = force_response->y;
  }
  else{
    status = f.wait_for(1ms);
    goto check_again;
  }
}
void ready_get(rclcpp::Client<service_interface::srv::Ready>::SharedFuture &f, bool &ready)
{
  auto status = f.wait_for(1ms); // not spinning here!
  check_again:
  if (status == std::future_status::ready)
  {
    auto ready_response = f.get();
    ready = ready_response->ready;
  }
  else{
    status = f.wait_for(1ms);
    goto check_again;
  }
}
void force_boundary_get(rclcpp::Client<service_interface::srv::ForceBoundaryCheck>::SharedFuture &f, double &x, double &y, bool &is_overwritten)
{
  auto status = f.wait_for(1ms); // not spinning here!
  check_again:
  if (status == std::future_status::ready)
  {
    auto fb_response = f.get();
    x = fb_response->x;
    y = fb_response->y;
    is_overwritten = fb_response->is_overwritten;
  }
  else{
    status = f.wait_for(1ms);
    goto check_again;
  }
}
void velocity_boundary_get(rclcpp::Client<service_interface::srv::VelocityBoundaryCheck>::SharedFuture &f, double &x, double &y)
{
  auto status = f.wait_for(1ms); // not spinning here!
  check_again:
  if (status == std::future_status::ready)
  {
    auto vb_response = f.get();
    x = vb_response->x;
    y = vb_response->y;
  }
  else{
    status = f.wait_for(1ms);
    goto check_again;
  }
}

