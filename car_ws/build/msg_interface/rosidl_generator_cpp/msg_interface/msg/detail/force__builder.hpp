// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from msg_interface:msg/Force.idl
// generated code does not contain a copyright notice

#ifndef MSG_INTERFACE__MSG__DETAIL__FORCE__BUILDER_HPP_
#define MSG_INTERFACE__MSG__DETAIL__FORCE__BUILDER_HPP_

#include "msg_interface/msg/detail/force__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace msg_interface
{

namespace msg
{

namespace builder
{

class Init_Force_y
{
public:
  explicit Init_Force_y(::msg_interface::msg::Force & msg)
  : msg_(msg)
  {}
  ::msg_interface::msg::Force y(::msg_interface::msg::Force::_y_type arg)
  {
    msg_.y = std::move(arg);
    return std::move(msg_);
  }

private:
  ::msg_interface::msg::Force msg_;
};

class Init_Force_x
{
public:
  Init_Force_x()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Force_y x(::msg_interface::msg::Force::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_Force_y(msg_);
  }

private:
  ::msg_interface::msg::Force msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::msg_interface::msg::Force>()
{
  return msg_interface::msg::builder::Init_Force_x();
}

}  // namespace msg_interface

#endif  // MSG_INTERFACE__MSG__DETAIL__FORCE__BUILDER_HPP_
