#include <cstdio>
#include "msg_interface/msg/force.hpp"
#include "msg_interface/msg/velocity.hpp"
#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <atomic>
#include <mutex>
using namespace std::chrono_literals;
//fixed timestep should be moved to ros2 parameter server
float delta_t = 0.1;
struct velocity_control : public rclcpp::Node
{
  velocity_control() : Node("velocity_control")
  {
    //callback group for subscription
    cg[0] = this->create_callback_group(rclcpp::callback_group::CallbackGroupType::MutuallyExclusive);
    auto opt = rclcpp::SubscriptionOptions();
    opt.callback_group = cg[0];
    //callback group for timer
    cg[1] = this->create_callback_group(rclcpp::callback_group::CallbackGroupType::MutuallyExclusive);
    force_sub_ = this->create_subscription<msg_interface::msg::Force>(
        "/subsumption/force", 10, [this](msg_interface::msg::Force::SharedPtr msg)
        { this->force_callback(msg); },
        opt);
    timer_ = this->create_wall_timer(
        20ms, [this]()
        { this->timer_callback(); },
        cg[1]);
    velocity_pub_ = this->create_publisher<msg_interface::msg::Velocity>("/control/velocity", 10);
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Subscription<msg_interface::msg::Force>::SharedPtr force_sub_;
  rclcpp::Publisher<msg_interface::msg::Velocity>::SharedPtr velocity_pub_;
  rclcpp::CallbackGroup::SharedPtr cg[2];
  //mutual exclusion needed for force accumulation
  std::mutex mtx;
  // mtx provides critical section for @START
  double force_x_accum = 0.0;
  double force_y_accum = 0.0;
  uint64_t force_count = 0;
  // @END
  void force_callback(msg_interface::msg::Force::SharedPtr msg)
  {
    if ((msg->x * msg->x + msg->y * msg->y) != 0)
    {
      //if not zero message add
      mtx.lock();
      force_x_accum += msg->x;
      force_y_accum += msg->y;
      force_count++;
      mtx.unlock();
    }
  }
  void timer_callback()
  {
    msg_interface::msg::Velocity vel_msg;
    float x = 0;
    float y = 0;
    //average of msgs that came between last timer callback and this
    mtx.lock();
    if (force_count)
    {
      x = force_x_accum / force_count;
      y = force_y_accum / force_count;
    }
    force_x_accum = 0;
    force_y_accum = 0;
    force_count = 0;
    mtx.unlock();
    // force to velocity conversion
    vel_msg.x = x * delta_t;
    vel_msg.y = y * delta_t;
    velocity_pub_->publish(vel_msg);
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor;
  auto node = std::make_shared<velocity_control>();
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
