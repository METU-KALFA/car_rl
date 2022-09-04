// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from service_interface:srv/PolyLineIntersectionCheck.idl
// generated code does not contain a copyright notice

#ifndef SERVICE_INTERFACE__SRV__DETAIL__POLY_LINE_INTERSECTION_CHECK__BUILDER_HPP_
#define SERVICE_INTERFACE__SRV__DETAIL__POLY_LINE_INTERSECTION_CHECK__BUILDER_HPP_

#include "service_interface/srv/detail/poly_line_intersection_check__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace service_interface
{

namespace srv
{

namespace builder
{

class Init_PolyLineIntersectionCheck_Request_yaw
{
public:
  explicit Init_PolyLineIntersectionCheck_Request_yaw(::service_interface::srv::PolyLineIntersectionCheck_Request & msg)
  : msg_(msg)
  {}
  ::service_interface::srv::PolyLineIntersectionCheck_Request yaw(::service_interface::srv::PolyLineIntersectionCheck_Request::_yaw_type arg)
  {
    msg_.yaw = std::move(arg);
    return std::move(msg_);
  }

private:
  ::service_interface::srv::PolyLineIntersectionCheck_Request msg_;
};

class Init_PolyLineIntersectionCheck_Request_y
{
public:
  explicit Init_PolyLineIntersectionCheck_Request_y(::service_interface::srv::PolyLineIntersectionCheck_Request & msg)
  : msg_(msg)
  {}
  Init_PolyLineIntersectionCheck_Request_yaw y(::service_interface::srv::PolyLineIntersectionCheck_Request::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_PolyLineIntersectionCheck_Request_yaw(msg_);
  }

private:
  ::service_interface::srv::PolyLineIntersectionCheck_Request msg_;
};

class Init_PolyLineIntersectionCheck_Request_x
{
public:
  Init_PolyLineIntersectionCheck_Request_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_PolyLineIntersectionCheck_Request_y x(::service_interface::srv::PolyLineIntersectionCheck_Request::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_PolyLineIntersectionCheck_Request_y(msg_);
  }

private:
  ::service_interface::srv::PolyLineIntersectionCheck_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::service_interface::srv::PolyLineIntersectionCheck_Request>()
{
  return service_interface::srv::builder::Init_PolyLineIntersectionCheck_Request_x();
}

}  // namespace service_interface


namespace service_interface
{

namespace srv
{

namespace builder
{

class Init_PolyLineIntersectionCheck_Response_dist
{
public:
  Init_PolyLineIntersectionCheck_Response_dist()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::service_interface::srv::PolyLineIntersectionCheck_Response dist(::service_interface::srv::PolyLineIntersectionCheck_Response::_dist_type arg)
  {
    msg_.dist = std::move(arg);
    return std::move(msg_);
  }

private:
  ::service_interface::srv::PolyLineIntersectionCheck_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::service_interface::srv::PolyLineIntersectionCheck_Response>()
{
  return service_interface::srv::builder::Init_PolyLineIntersectionCheck_Response_dist();
}

}  // namespace service_interface

#endif  // SERVICE_INTERFACE__SRV__DETAIL__POLY_LINE_INTERSECTION_CHECK__BUILDER_HPP_
