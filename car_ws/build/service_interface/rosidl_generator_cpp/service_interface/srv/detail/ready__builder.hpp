// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from service_interface:srv/Ready.idl
// generated code does not contain a copyright notice

#ifndef SERVICE_INTERFACE__SRV__DETAIL__READY__BUILDER_HPP_
#define SERVICE_INTERFACE__SRV__DETAIL__READY__BUILDER_HPP_

#include "service_interface/srv/detail/ready__struct.hpp"
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
auto build<::service_interface::srv::Ready_Request>()
{
  return ::service_interface::srv::Ready_Request(rosidl_runtime_cpp::MessageInitialization::ZERO);
}

}  // namespace service_interface


namespace service_interface
{

namespace srv
{

namespace builder
{

class Init_Ready_Response_ready
{
public:
  Init_Ready_Response_ready()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::service_interface::srv::Ready_Response ready(::service_interface::srv::Ready_Response::_ready_type arg)
  {
    msg_.ready = std::move(arg);
    return std::move(msg_);
  }

private:
  ::service_interface::srv::Ready_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::service_interface::srv::Ready_Response>()
{
  return service_interface::srv::builder::Init_Ready_Response_ready();
}

}  // namespace service_interface

#endif  // SERVICE_INTERFACE__SRV__DETAIL__READY__BUILDER_HPP_
