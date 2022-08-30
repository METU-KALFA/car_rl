// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from msg_interface:msg/Force.idl
// generated code does not contain a copyright notice

#ifndef MSG_INTERFACE__MSG__DETAIL__FORCE__TRAITS_HPP_
#define MSG_INTERFACE__MSG__DETAIL__FORCE__TRAITS_HPP_

#include "msg_interface/msg/detail/force__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<msg_interface::msg::Force>()
{
  return "msg_interface::msg::Force";
}

template<>
inline const char * name<msg_interface::msg::Force>()
{
  return "msg_interface/msg/Force";
}

template<>
struct has_fixed_size<msg_interface::msg::Force>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<msg_interface::msg::Force>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<msg_interface::msg::Force>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // MSG_INTERFACE__MSG__DETAIL__FORCE__TRAITS_HPP_
