#include <cstdio>
#include <rclcpp/rclcpp.hpp>
#include "service_interface/srv/velocity_boundary_check.hpp"
#include "msg_interface/msg/velocity.hpp"
#include "msg_interface/msg/control.hpp"
#include <chrono>
#include <mutex>
#include <cmath>


using namespace std::chrono_literals;

struct subsumption_velocity: public rclcpp::Node
{
  subsumption_velocity(): rclcpp::Node("subsumption_velocity"){
    // callback groups for timer, services, and subscriptions
    for (size_t i = 0; i < 4; i++)
    {
      callback_groups[i] = this->create_callback_group(rclcpp::callback_group::CallbackGroupType::MutuallyExclusive);
    }
    auto opt = rclcpp::SubscriptionOptions();
    opt.callback_group = callback_groups[2];
    timer = this->create_wall_timer(100ms, std::bind(&subsumption_velocity::timer_callback, this), callback_groups[0]);
    vel_lane_check = this->create_client<service_interface::srv::VelocityBoundaryCheck>("/subsumption_vel/lane",rmw_qos_profile_services_default, callback_groups[1]);
    vel_sub = this->create_subscription<msg_interface::msg::Velocity>("/control/velocity", 10, [this](msg_interface::msg::Velocity::SharedPtr msg) {
      this->vel_callback(msg);
    }, opt);
    vel_pub = this->create_publisher<msg_interface::msg::Control>("/control", 10);
  }
  rclcpp::TimerBase::SharedPtr timer;
  rclcpp::Client<service_interface::srv::VelocityBoundaryCheck>::SharedPtr vel_lane_check;
  rclcpp::Subscription<msg_interface::msg::Velocity>::SharedPtr vel_sub;
  rclcpp::Publisher<msg_interface::msg::Control>::SharedPtr vel_pub;
  rclcpp::CallbackGroup::SharedPtr callback_groups[4];
  // mutual exclusion for velocity accumulation
  std::mutex mtx;
  // mtx provides critical section for @START
  double vel_x_accum = 0;
  double vel_y_accum = 0;
  uint64_t vel_count = 0;
  // @END
  double published_x=0;
  double published_y=0;
  // previously published values
  void vel_callback(msg_interface::msg::Velocity::SharedPtr msg){
    if(msg->x != 0.0 || msg->y != 0.0){
      // if message is not 0 add
      mtx.lock();
        vel_x_accum+= msg->x;
        vel_y_accum+= msg->y;
        vel_count++;
      mtx.unlock();
    }
  }
  void timer_callback(){
    float x=0,y=0;
    //take the average of msgs that came between last timer callback and this callback
    mtx.lock();
      if(vel_count){
        x = vel_x_accum/vel_count;
        y = vel_y_accum/vel_count;
      }
      vel_x_accum = 0;
      vel_y_accum = 0;
      vel_count = 0;
    mtx.unlock();
    // 45 degree angle is allowed in trx6 thus we have cuts here
    float alpha = atan2(y,x);
    if(alpha>(3.14/4) || alpha < (-3.14/4)){
      x=abs(y)*1.05; // dont steer the whole value
      // less than 45 degree steering always
    }
    // @ATTENTION A subsumption logic can be added here to provide adjustment of values if they do not adhere to the desired outcome.
    // An example of how to implement such logic can be found in $subsumption_force$ node
    msg_interface::msg::Control vel_msg;
    vel_msg.vx = x;
    vel_msg.vy = y;
    if(x!=0.0 || y !=0.0){
      // if not zero post the msg and update published
      // published_x and published_y are used only here and the timer callback is not reentrant.
      // if in future for any reason one decides to modify these variables outside of the callback group of timer, one needs to take care of the data race that would occur in such a case.
      vel_pub->publish(vel_msg);
      published_x = x;
      published_y = y;
      
    }else{
      // if a zero message has arrived (lane is not detected in the last timer callback period), carry on with theold value
      vel_msg.vx = published_x;
      vel_msg.vy = published_y;
      vel_pub->publish(vel_msg);
      // ideally one would wish to eliminate this by using a fast enough lane detector.
    }
    
  }

};



int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor;
  auto node = std::make_shared<subsumption_velocity>();
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
