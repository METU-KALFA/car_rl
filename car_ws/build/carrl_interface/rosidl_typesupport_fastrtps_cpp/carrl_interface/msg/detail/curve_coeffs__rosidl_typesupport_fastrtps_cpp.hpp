// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice

#ifndef CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "carrl_interface/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "carrl_interface/msg/detail/curve_coeffs__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace carrl_interface
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_carrl_interface
cdr_serialize(
  const carrl_interface::msg::CurveCoeffs & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_carrl_interface
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  carrl_interface::msg::CurveCoeffs & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_carrl_interface
get_serialized_size(
  const carrl_interface::msg::CurveCoeffs & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_carrl_interface
max_serialized_size_CurveCoeffs(
  bool & full_bounded,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace carrl_interface

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_carrl_interface
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, carrl_interface, msg, CurveCoeffs)();

#ifdef __cplusplus
}
#endif

#endif  // CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
