// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from msg_interface:msg/Control.idl
// generated code does not contain a copyright notice

#ifndef MSG_INTERFACE__MSG__DETAIL__CONTROL__BUILDER_HPP_
#define MSG_INTERFACE__MSG__DETAIL__CONTROL__BUILDER_HPP_

#include "msg_interface/msg/detail/control__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace msg_interface
{

namespace msg
{

namespace builder
{

class Init_Control_vy
{
public:
  explicit Init_Control_vy(::msg_interface::msg::Control & msg)
  : msg_(msg)
  {}
  ::msg_interface::msg::Control vy(::msg_interface::msg::Control::_vy_type arg)
  {
    msg_.vy = std::move(arg);
    return std::move(msg_);
  }

private:
  ::msg_interface::msg::Control msg_;
};

class Init_Control_vx
{
public:
  Init_Control_vx()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Control_vy vx(::msg_interface::msg::Control::_vx_type arg)
  {
    msg_.vx = std::move(arg);
    return Init_Control_vy(msg_);
  }

private:
  ::msg_interface::msg::Control msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::msg_interface::msg::Control>()
{
  return msg_interface::msg::builder::Init_Control_vx();
}

}  // namespace msg_interface

#endif  // MSG_INTERFACE__MSG__DETAIL__CONTROL__BUILDER_HPP_
