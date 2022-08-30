// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from service_interface:srv/Force.idl
// generated code does not contain a copyright notice

#ifndef SERVICE_INTERFACE__SRV__DETAIL__FORCE__BUILDER_HPP_
#define SERVICE_INTERFACE__SRV__DETAIL__FORCE__BUILDER_HPP_

#include "service_interface/srv/detail/force__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace service_interface
{

namespace srv
{


}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::service_interface::srv::Force_Request>()
{
  return ::service_interface::srv::Force_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace service_interface


namespace service_interface
{

namespace srv
{

namespace builder
{

class Init_Force_Response_y
{
public:
  explicit Init_Force_Response_y(::service_interface::srv::Force_Response & msg)
  : msg_(msg)
  {}
  ::service_interface::srv::Force_Response y(::service_interface::srv::Force_Response::_y_type arg)
  {
    msg_.y = std::move(arg);
    return std::move(msg_);
  }

private:
  ::service_interface::srv::Force_Response msg_;
};

class Init_Force_Response_x
{
public:
  Init_Force_Response_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Force_Response_y x(::service_interface::srv::Force_Response::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_Force_Response_y(msg_);
  }

private:
  ::service_interface::srv::Force_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::service_interface::srv::Force_Response>()
{
  return service_interface::srv::builder::Init_Force_Response_x();
}

}  // namespace service_interface

#endif  // SERVICE_INTERFACE__SRV__DETAIL__FORCE__BUILDER_HPP_
