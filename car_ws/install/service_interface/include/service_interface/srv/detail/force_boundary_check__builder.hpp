// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from service_interface:srv/ForceBoundaryCheck.idl
// generated code does not contain a copyright notice

#ifndef SERVICE_INTERFACE__SRV__DETAIL__FORCE_BOUNDARY_CHECK__BUILDER_HPP_
#define SERVICE_INTERFACE__SRV__DETAIL__FORCE_BOUNDARY_CHECK__BUILDER_HPP_

#include "service_interface/srv/detail/force_boundary_check__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace service_interface
{

namespace srv
{

namespace builder
{

class Init_ForceBoundaryCheck_Request_y
{
public:
  explicit Init_ForceBoundaryCheck_Request_y(::service_interface::srv::ForceBoundaryCheck_Request & msg)
  : msg_(msg)
  {}
  ::service_interface::srv::ForceBoundaryCheck_Request y(::service_interface::srv::ForceBoundaryCheck_Request::_y_type arg)
  {
    msg_.y = std::move(arg);
    return std::move(msg_);
  }

private:
  ::service_interface::srv::ForceBoundaryCheck_Request msg_;
};

class Init_ForceBoundaryCheck_Request_x
{
public:
  Init_ForceBoundaryCheck_Request_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ForceBoundaryCheck_Request_y x(::service_interface::srv::ForceBoundaryCheck_Request::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_ForceBoundaryCheck_Request_y(msg_);
  }

private:
  ::service_interface::srv::ForceBoundaryCheck_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::service_interface::srv::ForceBoundaryCheck_Request>()
{
  return service_interface::srv::builder::Init_ForceBoundaryCheck_Request_x();
}

}  // namespace service_interface


namespace service_interface
{

namespace srv
{

namespace builder
{

class Init_ForceBoundaryCheck_Response_y
{
public:
  explicit Init_ForceBoundaryCheck_Response_y(::service_interface::srv::ForceBoundaryCheck_Response & msg)
  : msg_(msg)
  {}
  ::service_interface::srv::ForceBoundaryCheck_Response y(::service_interface::srv::ForceBoundaryCheck_Response::_y_type arg)
  {
    msg_.y = std::move(arg);
    return std::move(msg_);
  }

private:
  ::service_interface::srv::ForceBoundaryCheck_Response msg_;
};

class Init_ForceBoundaryCheck_Response_x
{
public:
  explicit Init_ForceBoundaryCheck_Response_x(::service_interface::srv::ForceBoundaryCheck_Response & msg)
  : msg_(msg)
  {}
  Init_ForceBoundaryCheck_Response_y x(::service_interface::srv::ForceBoundaryCheck_Response::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_ForceBoundaryCheck_Response_y(msg_);
  }

private:
  ::service_interface::srv::ForceBoundaryCheck_Response msg_;
};

class Init_ForceBoundaryCheck_Response_is_overwritten
{
public:
  Init_ForceBoundaryCheck_Response_is_overwritten()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ForceBoundaryCheck_Response_x is_overwritten(::service_interface::srv::ForceBoundaryCheck_Response::_is_overwritten_type arg)
  {
    msg_.is_overwritten = std::move(arg);
    return Init_ForceBoundaryCheck_Response_x(msg_);
  }

private:
  ::service_interface::srv::ForceBoundaryCheck_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::service_interface::srv::ForceBoundaryCheck_Response>()
{
  return service_interface::srv::builder::Init_ForceBoundaryCheck_Response_is_overwritten();
}

}  // namespace service_interface

#endif  // SERVICE_INTERFACE__SRV__DETAIL__FORCE_BOUNDARY_CHECK__BUILDER_HPP_
