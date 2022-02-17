//#include "ros/ros.hpp"
//#include "ros/console.h"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <tf2/transform_datatypes.h>
#include "tf2_msgs/msg/tf_message.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "nav_msgs/msg/odometry.hpp"
#include "odometry.h"
#include "rclcpp/rclcpp.hpp"
#include <tf2_ros/transform_broadcaster.h>
#include <sstream>
#include <iostream> // std::cout
#include <string>
#include <math.h>
using namespace std::chrono_literals;
constexpr double pose_cov_list[] = {0.001, 0.001, 0.001, 0.001, 0.001, 0.01};
constexpr double twist_cov_list[] = {0.001, 0.001, 0.001, 0.001, 0.001, 0.01};
struct vec3
{
  double x;
  double y;
  double z;
};
void normal(std::vector<double> &pos)
{
  for (int i = 0; i < 8; i++)
  {
    pos[i] -= floor(pos[i] / M_PI) * M_PI;
  }
}
struct TwistToAackermann : public rclcpp::Node
{
  const double wheelBase = 0.5;
  const double track = 0.025;
  const double wheelMiddleBase = 0.3;
  const double wheelRadius = 0.12;
  const double actualBase = (wheelBase + wheelMiddleBase) / 2;
  const double trackMul = 1;
  const double wheelRadiusMul = 1;
  double yaw = 0;
  double slip = 0;
  vec3 pose = {0, 0, 0};
  TwistToAackermann() : Node("ackermann_steering_control"), pub_(create_publisher<sensor_msgs::msg::JointState>("/joint_states", 1)),
                        pub_odom_(create_publisher<nav_msgs::msg::Odometry>("/wheel_odom", 1)),
                        pub_odom_tf_(create_publisher<tf2_msgs::msg::TFMessage>("/odom_tf", 1)),
                        sub_(create_subscription<geometry_msgs::msg::Twist>("/cmd_vel", 1, std::bind(&TwistToAackermann::callback, this, std::placeholders::_1)))
  {
    //Topic you want to publish
    tf_publisher_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
    delta_t = 0.1;
    timer_ = this->create_wall_timer(100ms, std::bind(&TwistToAackermann::broadcast_timer_callback, this));
    joint.header.frame_id = std::string("");
    joint.name = {"drivewhl_rl_joint", "drivewhl_rr_joint",
                  "drivewhl_fl_joint", "drivewhl_fr_joint",
                  "drivewhl_ml_joint", "drivewhl_mr_joint",
                  "left_steering_hinge_joint", "right_steering_hinge_joint"};
    joint.effort = {10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0, 10.0};
    joint.velocity = {0, 0, 0, 0, 0, 0, 0, 0};
    joint.position = {0, 0, 0, 0, 0, 0, 0, 0};
    odometry_.setVelocityRollingWindowSize(10);
    const double ws_h = track * trackMul;
    const double wr = wheelRadiusMul * wheelRadius;
    odometry_.setWheelParams(ws_h, wr);
    msg_.header.frame_id = "odom";
    msg_.child_frame_id = "base_link";
    msg_.pose.pose.position.z = 0;
    msg_.pose.covariance = {
        static_cast<double>(pose_cov_list[0]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(pose_cov_list[1]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(pose_cov_list[2]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(pose_cov_list[3]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(pose_cov_list[4]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(pose_cov_list[5]), 0., 0., 0., 0., 0., 0.};
    msg_.twist.twist.linear.y = 0;
    msg_.twist.twist.linear.z = 0;
    msg_.twist.twist.angular.x = 0;
    msg_.twist.twist.angular.y = 0;
    msg_.twist.covariance = {
        static_cast<double>(twist_cov_list[0]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(twist_cov_list[1]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(twist_cov_list[2]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(twist_cov_list[3]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(twist_cov_list[4]), 0., 0., 0., 0., 0., 0.,
        static_cast<double>(twist_cov_list[5]), 0., 0., 0., 0., 0., 0.};
    tf_msg_.transforms.resize(1);
    tf_msg_.transforms[0].transform.translation.z = 0.0;
    tf_msg_.transforms[0].child_frame_id = "base_link";
    tf_msg_.transforms[0].header.frame_id = "odom";
  }

  void broadcast_timer_callback()
  {
    rclcpp::Time now = this->get_clock()->now();
    geometry_msgs::msg::TransformStamped t = tf_msg_.transforms[0];

    t.header.stamp = now;
    t.header.frame_id = "odom";
    t.child_frame_id = "base_link";

    const geometry_msgs::msg::Quaternion orientation(createQuaternionMsgFromYaw(yaw));

    t.transform.translation.x = pose.x;
    t.transform.translation.y = pose.y;
    t.transform.rotation = orientation;

    tf_publisher_->sendTransform(t);
  }

  geometry_msgs::msg::Quaternion createQuaternionMsgFromYaw(double yaw)
  {
    tf2::Quaternion q;
    q.setRPY(0, 0, yaw);
    return tf2::toMsg(q);
  }
  double wheel_pos_old = 0;
  double steer_pos_old = 0;
  double delta_t;
  void callback(const geometry_msgs::msg::Twist::SharedPtr msg)
  {
    // Vehicle wheel base geometry. wheelBase is the distance between front
    // and back axles, track is the distance between left and right wheels.
    //double MaxR = 3.81;
    // sensor_msgs::JointState joint;
    double lwh;
    double rwh;
    double lwr;
    double rwr;
    double lwm;
    double rwm;
    double lwf;
    double rwf;
    double r = 0;
    double tanz = 0;
    joint.header.stamp = now();
    if (msg->angular.z == 0 || msg->linear.x == 0)
    {
      lwh = 0;
      rwh = 0;
      rwr = lwr = rwf = lwf = rwm = lwm = linearToAngularVelocity(msg->linear.x, wheelRadius);
      slip = 0;
    }
    else
    {
      // The radius of the turn.
      tanz = tan(msg->angular.z);
      r = actualBase / tanz;
      slip = atan(tanz / 2);
      // ROS_DEBUG("%f\n", r);
      //if (r<MaxR && r>0) r = MaxR;
      //if (r>-MaxR && r<0) r = -MaxR;

      lwh = atan(actualBase / (r - (track / 2)));
      rwh = atan(actualBase / (r + (track / 2)));
      lwf = lwm = lwr = linearToAngularVelocity((r - (track / 2)) * msg->angular.z, wheelRadius);
      rwf = rwm = rwr = linearToAngularVelocity((r + (track / 2)) * msg->angular.z, wheelRadius);
    }
    joint.position[0] += lwr;
    joint.position[1] += rwr;
    joint.position[2] += lwf;
    joint.position[3] += rwf;
    joint.position[4] += lwm;
    joint.position[5] += rwm;
    joint.position[6] = lwh;
    joint.position[7] = rwh;
    normal(joint.position);
    double wheel_pos = wheel_pos_old + msg->linear.x * delta_t;
    double steer_pos = steer_pos_old + (lwh + rwh) / 2;
    //odometry_.update(wheel_pos, steer_pos, joint.header.stamp);
    wheel_pos_old = wheel_pos;
    steer_pos_old = steer_pos;
    pose.x += msg->linear.x * cos(yaw + slip) * delta_t;
    pose.y += msg->linear.x * sin(yaw + slip) * delta_t;
    if (r != 0)
    {
      yaw += delta_t * (msg->linear.x / r);
    }
    const geometry_msgs::msg::Quaternion orientation(
        createQuaternionMsgFromYaw(yaw));
    msg_.header.stamp = joint.header.stamp;
    msg_.pose.pose.position.x = pose.x;
    msg_.pose.pose.position.y = pose.y;
    msg_.pose.pose.orientation = orientation;
    msg_.twist.twist.linear.x = msg->linear.x;
    msg_.twist.twist.angular.z = msg->angular.z;

    {

      geometry_msgs::msg::TransformStamped &odom_frame = tf_msg_.transforms[0];
      odom_frame.header.stamp = joint.header.stamp;
      odom_frame.transform.translation.x = pose.x;
      odom_frame.transform.translation.y = pose.y;
      odom_frame.transform.rotation = orientation;

      //    geometry_msgs::msg::TransformStamped t = tf_msg_.transforms[0];

      odom_frame.header.frame_id = "odom";
      odom_frame.child_frame_id = "base_link";

      tf_publisher_->sendTransform(odom_frame);
    }
    //double R = static_cast<double>(msg.linear.x/msg.angular.z);
    //ROS_DEBUG("Test");

    //pubADS_.publish(ads);
    pub_odom_->publish(msg_);
    pub_odom_tf_->publish(tf_msg_);
    pub_->publish(joint);
  }
  nav_msgs::msg::Odometry msg_;
  tf2_msgs::msg::TFMessage tf_msg_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_;
  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr pub_odom_;
  rclcpp::Publisher<tf2_msgs::msg::TFMessage>::SharedPtr pub_odom_tf_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_publisher_;
  sensor_msgs::msg::JointState joint;
  Odometry odometry_;
  float linearToAngularVelocity(float linearVelocity, float radius)
  {
    return linearVelocity / radius;
  }
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  rclcpp::spin(std::make_shared<TwistToAackermann>());
  rclcpp::shutdown();
  return 0;
}
