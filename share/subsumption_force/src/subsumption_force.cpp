#include <cstdio>
#include <rclcpp/rclcpp.hpp>
#include <msg_interface/msg/force.hpp>
#include "service_interface/srv/force.hpp"
#include "service_interface/srv/ready.hpp"
#include "service_interface/srv/force_boundary_check.hpp"
#include "service_interface/srv/location.hpp"
#include "service_client_lib/service_calls.hpp"
#include <chrono>
#include <atomic>
#include <cmath>
#include <semaphore.h>
using namespace std::chrono_literals;
struct subsumption_force : public rclcpp::Node
{
  subsumption_force() : rclcpp::Node("subsumption_force")
  {
    // timer should be reentrant to not pile up at this rate
    callback_groups[0] = this->create_callback_group(rclcpp::callback_group::CallbackGroupType::Reentrant);
    // callback groups for service clients.
    for (size_t i = 1; i < 7; i++)
    {
      callback_groups[i] = this->create_callback_group(rclcpp::callback_group::CallbackGroupType::MutuallyExclusive);
    }

    timer = this->create_wall_timer(17ms, std::bind(&subsumption_force::timer_callback, this), callback_groups[0]);
    location_client = this->create_client<service_interface::srv::Location>("/service/odom", rmw_qos_profile_services_default, callback_groups[1]);
    ready_goal_client = this->create_client<service_interface::srv::Ready>("/ready/goal", rmw_qos_profile_services_default, callback_groups[2]);
    force_goal_client = this->create_client<service_interface::srv::Force>("/force/goal", rmw_qos_profile_services_default, callback_groups[3]);
    ready_lane_client = this->create_client<service_interface::srv::Ready>("/ready/lane", rmw_qos_profile_services_default, callback_groups[4]);
    force_lane_client = this->create_client<service_interface::srv::Force>("/force/lane", rmw_qos_profile_services_default, callback_groups[5]);
    force_goal_boundary_check_client = this->create_client<service_interface::srv::ForceBoundaryCheck>("/subsumption_force/goal", rmw_qos_profile_services_default, callback_groups[6]);
    
    force_pub = this->create_publisher<msg_interface::msg::Force>("/subsumption/force", 10);
    // Do not start processing till every service we need is ready...
    // very important, please add wait_for_service() calls to any new nodes that you are writing.
    // @COPY: copy this comment to nodes that have service clients to wait for.
    location_client->wait_for_service();
    ready_goal_client->wait_for_service();
    force_goal_client->wait_for_service();
    ready_lane_client->wait_for_service();
    force_lane_client->wait_for_service();
    force_goal_boundary_check_client->wait_for_service();
  }
  rclcpp::TimerBase::SharedPtr timer;
  rclcpp::Client<service_interface::srv::Location>::SharedPtr location_client;
  rclcpp::Client<service_interface::srv::Ready>::SharedPtr ready_goal_client;
  rclcpp::Client<service_interface::srv::Force>::SharedPtr force_goal_client;
  rclcpp::Client<service_interface::srv::Ready>::SharedPtr ready_lane_client;
  rclcpp::Client<service_interface::srv::Force>::SharedPtr force_lane_client;
  rclcpp::Client<service_interface::srv::ForceBoundaryCheck>::SharedPtr force_goal_boundary_check_client;
  rclcpp::Publisher<msg_interface::msg::Force>::SharedPtr force_pub;
  rclcpp::CallbackGroup::SharedPtr callback_groups[9];
  void timer_callback()
  {
    msg_interface::msg::Force msg;
    bool ready[2] = {false, false};
    rclcpp::Client<service_interface::srv::Location>::SharedRequest client_request = std::make_shared<service_interface::srv::Location::Request>();
    rclcpp::Client<service_interface::srv::Ready>::SharedRequest ready_request = std::make_shared<service_interface::srv::Ready::Request>();
    double odom_x, odom_y, odom_yaw;
    
    //async requests... Note that as all have different callback groups all will have their own threads. 
    auto ready_1_srv = ready_goal_client->async_send_request(ready_request);
    auto ready_2_srv = ready_goal_client->async_send_request(ready_request);
    auto location_srv = location_client->async_send_request(client_request);
    // If you are to implement a similar subsumption logic check the implementation of these functions.
    // It is likely that you will need another service interface to do so go to the directory( service_interface/srv/ ) and create a new service file you can take a look at what is already inside the directory
    // or consult the internet. https://docs.ros.org/en/foxy/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Service-And-Client.html
    // please implement service calls like they are implemented in "service_client_lib/service_calls.hpp", otherwise you will find that async service calls are very tricky to write,
    // and probably write ugly code that needs to be general, all over the place. Check that your services do not block after you are done writing them.
    ready_get(ready_1_srv,ready[0]);
    ready_get(ready_2_srv,ready[1]);
    if (!ready[0] && !ready[1])
    {
      //no one is ready to get force requests
      location_get(location_srv,odom_x,odom_y,odom_yaw);
      return;
    }
    location_get(location_srv,odom_x,odom_y,odom_yaw);
    
    rclcpp::Client<service_interface::srv::Force>::SharedRequest force_request = std::make_shared<service_interface::srv::Force::Request>();
    if (ready[0] && ready[1])
    {
      // goal and lane are ready
      //  in this case we either subsume the goal force by lane force or clip the goal force

      double x, y;
      
      auto force_srv = force_goal_client->async_send_request(force_request);
      force_get(force_srv,x,y);
      //x and y now have the force output from goal node
      rclcpp::Client<service_interface::srv::ForceBoundaryCheck>::SharedRequest force_boundary_check_request = std::make_shared<service_interface::srv::ForceBoundaryCheck::Request>();
      force_boundary_check_request->x = x;
      force_boundary_check_request->y = y;
      bool changed = false;

      auto force_boundary_check_srv = force_goal_boundary_check_client->async_send_request(force_boundary_check_request);
      force_boundary_get(force_boundary_check_srv,x,y,changed);
      //x and y now have the corrected force output from the lane_boundary node or if it was not in a certain range in the (Float X Float) space it was not changed.
      if (changed)
      {
        // clipped goal force
        msg.x = x;
        msg.y = y;
      }
      else
      {
        // The input to lane subsumption was not in the satisfactory range and thus the lane force completely overwrites the output
        // subsume goal force by lane force
        auto force_srv = force_lane_client->async_send_request(force_request);
        force_get(force_srv,x,y);
        msg.x = x;
        msg.y = y;
      }
    }
    else if (ready[1])
    {
      // lane and not goal
      // goal node is not ready to be probed, in this case we go on with lane force
      auto force_srv = force_lane_client->async_send_request(force_request);
      double x,y;
      force_get(force_srv,x,y);
      msg.x = x;
      msg.y = y;
    }
    else
    {
      // not lane and goal
      // goal is ready however we do not take it into account on its own as such a case might make us go out of lane
      // which is a property we would like to enforce 
      return;
    }
    //force is in local coordinates
    force_pub->publish(msg);
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::executors::MultiThreadedExecutor executor;
  auto node = std::make_shared<subsumption_force>();
  executor.add_node(node);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
