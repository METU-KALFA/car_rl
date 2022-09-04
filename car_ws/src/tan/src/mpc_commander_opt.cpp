#include "rclcpp/rclcpp.hpp"
#include "rclcpp/qos.hpp"
#include "mavros_msgs/srv/set_mode.hpp"
#include "mavros_msgs/srv/command_bool.hpp"
#include "mavros_msgs/msg/state.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "geometry_msgs/msg/pose_array.hpp"
#include <geometry_msgs/msg/transform_stamped.hpp>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/utils.h>
#include "Eigen/Dense" // For matrix mult.
#include <nlopt.hpp> // Non-linear optimization

#include <memory>
#include <chrono>
#include <functional>
#include <vector>
#include <algorithm>

#define MAX_V 1.8 // m/s

using std::placeholders::_1;
using namespace std::chrono_literals;
struct fdata{
  double* x;
  double* y;
  double yaw;
  Eigen::MatrixX2d dest;
};

std::shared_ptr<rclcpp::Node> node;
std::shared_ptr<rclcpp::Publisher<geometry_msgs::msg::TwistStamped>> vel_pub;
geometry_msgs::msg::TwistStamped vel;
nav_msgs::msg::Odometry odom;
mavros_msgs::msg::State state;
geometry_msgs::msg::PoseArray path;
tf2::Quaternion quat;
std::vector<double> actionSet,lower_b,upper_b;
nlopt::opt global_optimizer,local_optimizer;
double yaw,p,r;
double max_v,car_length;
int event_horizon,action_horizon;
bool new_odom = false;
fdata mpc_data;

double fc_mpc(const std::vector<double> &x, std::vector<double> &grad, void* f_data){
  double penalty(0);
  for(int i = 0; i < 2*action_horizon; i=i+2){
    penalty += std::max(0.0,x[i+1] - x[i]/car_length) + std::max(0.0,x[i]-max_v) + std::max(0.0,0.3-x[i]);
  }
  return penalty;
}

inline Eigen::MatrixX2d msg2matrix(const geometry_msgs::msg::PoseArray &msg){
  Eigen::MatrixX2d m(event_horizon,2);
  for(unsigned int i = 0; i < (unsigned) event_horizon; i++){
    if(i < msg.poses.size()){
      m(i,0) = msg.poses[i].position.x;
      m(i,1) = msg.poses[i].position.y;
    }else{
      m.block<1,2>(i,0) = m.block<1,2>(i-1,0);
    }
  }
  return m;
}

inline Eigen::RowVector3d translation2d(const Eigen::Ref<const Eigen::RowVector3d> &s, const double &vx, const double &wz, const double &dt = 0.1){
  double dr = vx*cos(wz*dt/(2*1.735))*dt; // Approx. of 2*vx*sin(wz*dt/2)/wz, good for -10<wz<10 (to aviod zero div. when wz=0)
  return s + Eigen::RowVector3d{dr*cos(s(2)+wz*dt), dr*sin(s(2)+wz*dt), wz*dt};
}

inline Eigen::MatrixX2d model_iterate(const Eigen::Ref<const Eigen::RowVector3d> &init, const std::vector<double> &actionSet){
  Eigen::MatrixX2d points(event_horizon,2);
  Eigen::RowVector3d next = init;
  for(int i = 0; i < event_horizon; i=i+2){
    next = translation2d(next, actionSet[std::min(i,((int)actionSet.size())-1)],actionSet[std::min(i+1,((int)actionSet.size())-1)]);
    points.block<1,2>(i,0) = next.block<1,2>(0,0);
  }
  return points;
}

inline double calc_path_cost(const Eigen::Ref<const Eigen::MatrixX2d> &p1, const Eigen::Ref<const Eigen::MatrixX2d> &p2){
  return (p1 - p2).norm();
}

double mpc(const std::vector<double> &x, std::vector<double> &grad, void* f_data){
  Eigen::RowVector3d current{*(((fdata*)f_data)->x),*(((fdata*)f_data)->y), ((fdata*)f_data)->yaw};

  return calc_path_cost(model_iterate(current, x),((fdata*)f_data)->dest);
}

void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg){
  odom = *msg;
  //RCLCPP_INFO(node->get_logger(), "x: %.2f\ny: %.2f\nz: %.2f\n", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
  new_odom = true;
}

void state_callback(const mavros_msgs::msg::State::SharedPtr msg){
  state = *msg;
  //RCLCPP_INFO(node->get_logger(), "Connected: %d\tArmed: %d\tMode: %s",state.connected,state.armed,state.mode.c_str());
}

void path_callback(const geometry_msgs::msg::PoseArray::SharedPtr msg){
  path = *msg;
}


void timer_callback(){

}

int main(int argc, char * argv[]){
  // ROS Stuff
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("commander");
  node->declare_parameter<double>("max_v",1.0);
  node->declare_parameter<double>("car_length",0.35);
  node->declare_parameter<int>("event_horizon",20);
  node->declare_parameter<int>("action_horizon",4);
  node->get_parameter("max_v", max_v);
  node->get_parameter("car_length", car_length);
  node->get_parameter("event_horizon", event_horizon);
  node->get_parameter("action_horizon", action_horizon);
  rclcpp::Rate looprate(20);
  auto qos = rclcpp::QoS(1);
  qos.best_effort();
  // NLopt Stuff
  mpc_data.x = &(odom.pose.pose.position.x);
  mpc_data.y = &(odom.pose.pose.position.y);
  actionSet.reserve(action_horizon*2);
  lower_b.reserve(action_horizon*2);
  upper_b.reserve(action_horizon*2);
  for(int i = 0; i < action_horizon; i++){
    actionSet.emplace_back(1);
    actionSet.emplace_back(0);
    lower_b.emplace_back(0);
    lower_b.emplace_back(-MAX_V/car_length);
    upper_b.emplace_back( MAX_V);
    upper_b.emplace_back( MAX_V/car_length);
  }
  global_optimizer = nlopt::opt(nlopt::GN_ISRES, 2*action_horizon);
  global_optimizer.set_min_objective(mpc,&mpc_data);
  global_optimizer.set_lower_bounds(lower_b);
  global_optimizer.set_upper_bounds(upper_b);
  global_optimizer.add_inequality_constraint(fc_mpc, NULL, 0.001);
  global_optimizer.set_maxtime(0.005);
  local_optimizer = nlopt::opt(nlopt::LN_COBYLA, 2*action_horizon);
  local_optimizer.set_min_objective(mpc,&mpc_data);
  local_optimizer.set_lower_bounds(lower_b);
  local_optimizer.set_upper_bounds(upper_b);
  local_optimizer.add_inequality_constraint(fc_mpc, NULL, 0.001);
  local_optimizer.set_maxtime(0.001);
  //Mavros Stuff
  vel_pub = node->create_publisher<geometry_msgs::msg::TwistStamped>("/mavros/setpoint_velocity/cmd_vel", qos);
  auto set_mode_client = node->create_client<mavros_msgs::srv::SetMode>("/mavros/set_mode");
  auto arm_client = node->create_client<mavros_msgs::srv::CommandBool>("/mavros/cmd/arming");
  auto state_sub =  node->create_subscription<mavros_msgs::msg::State>(
                      "/mavros/state", 1, state_callback);
  auto odom_sub =  node->create_subscription<nav_msgs::msg::Odometry>(
                      "/mavros/global_position/local", qos, odom_callback);
  // For recieving path
  auto path_sub =  node->create_subscription<geometry_msgs::msg::PoseArray>(
                      "/commander/path", 1, path_callback);
  // Wait for connection
  RCLCPP_INFO(node->get_logger(), "Waiting to connect...");
  std::shared_ptr<rclcpp::executors::MultiThreadedExecutor> executor = rclcpp::executors::MultiThreadedExecutor::make_shared();
  executor->add_node(node);
  while ((!state.connected)&&rclcpp::ok()){
    looprate.sleep();
    executor->spin_some();
  }
  RCLCPP_INFO(node->get_logger(), "Connected!");
  // If we are not armed, then arm
  if(state.armed){
    RCLCPP_INFO(node->get_logger(), "Already Armed!");
  }else{
    auto arm_cmd = std::make_shared<mavros_msgs::srv::CommandBool::Request>();
    arm_cmd->value = true;

    while (!arm_client->wait_for_service(2s)) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the set_mode service. Exiting.");
        return 0;
      }
      RCLCPP_INFO(node->get_logger(), "set_mode service not available, waiting again...");
    }
    auto arm_result = arm_client->async_send_request(arm_cmd);
    // Wait for the result.
    if (executor->spin_until_future_complete(arm_result) ==
      rclcpp::FutureReturnCode::SUCCESS){
      if(arm_result.get()->success){
        RCLCPP_INFO(node->get_logger(), "ARMED");
      }else{
        RCLCPP_INFO(node->get_logger(), "ARM Failed");
      }
    }else{
      RCLCPP_ERROR(node->get_logger(), "Failed to call service /mavros/cmd/arming");
    }
  }
  //If the mode is not GUIDED, make it so
  if(state.guided){
      RCLCPP_INFO(node->get_logger(), "Already in Guided mode");
  }else{
    auto guided_mode = std::make_shared<mavros_msgs::srv::SetMode::Request>();
    guided_mode->custom_mode = "GUIDED";

    while (!set_mode_client->wait_for_service(2s)) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(node->get_logger(), "Interrupted while waiting for the set_mode service. Exiting.");
        return 0;
      }
      RCLCPP_INFO(node->get_logger(), "set_mode service not available, waiting again...");
    }
    auto set_mode_result = set_mode_client->async_send_request(guided_mode);
    // Wait for the result.
    if (executor->spin_until_future_complete(set_mode_result) ==
      rclcpp::FutureReturnCode::SUCCESS){
      if(set_mode_result.get()->mode_sent){
        RCLCPP_INFO(node->get_logger(), "Mode set to: GUIDED");
      }else{
        RCLCPP_INFO(node->get_logger(), "Mode couldn't set to: GUIDED");
      }
    }else{
      RCLCPP_ERROR(node->get_logger(), "Failed to call service /mavros/set_mode");
    }
  }
  auto timer_ = node->create_wall_timer(100ms, &timer_callback);
  RCLCPP_INFO(node->get_logger(), "Done");
  // Enter loop
  while(rclcpp::ok()){
    //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "x: %.2f\ny: %.2f\nz: %.2f\n", odom.pose.pose.position.x, odom.pose.pose.position.y, odom.pose.pose.position.z);
    auto message = geometry_msgs::msg::TwistStamped();
    if(new_odom){
      new_odom = false;
      if(path.poses.size()){
        tf2::fromMsg(odom.pose.pose.orientation, quat);
        tf2::Matrix3x3 matrix(quat);
        matrix.getEulerYPR(mpc_data.yaw,p,r);
        mpc_data.dest = msg2matrix(path);
        double optv(0);
        global_optimizer.optimize(actionSet,optv);
        try{
          local_optimizer.optimize(actionSet,optv);
        }catch (nlopt::roundoff_limited e){
          RCLCPP_WARN(node->get_logger(),"Resetting path: %s",e.what());
          //path.poses.clear();
        }
        RCLCPP_INFO(node->get_logger(), "Value: %.3f Action: %.7f, %.7f", optv, actionSet[0],actionSet[1]);
        message.twist.linear.x = actionSet[0];
        message.twist.angular.z = actionSet[1];
      }else{
        message.twist.linear.x = 0;
        message.twist.angular.z = 0;
      }
      vel_pub->publish(message);
    }
    looprate.sleep();
    executor->spin_some();
  }
  rclcpp::shutdown();
  return 0;
}