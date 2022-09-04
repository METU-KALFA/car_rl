// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__type_support.cpp.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice
#include "carrl_interface/msg/detail/curve_coeffs__rosidl_typesupport_fastrtps_cpp.hpp"
#include "carrl_interface/msg/detail/curve_coeffs__struct.hpp"

#include <limits>
#include <stdexcept>
#include <string>
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_fastrtps_cpp/wstring_conversion.hpp"
#include "fastcdr/Cdr.h"


// forward declaration of message dependencies and their conversion functions

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
  eprosima::fastcdr::Cdr & cdr)
{
  // Member: degree
  cdr << ros_message.degree;
  // Member: coeffs
  {
    cdr << ros_message.coeffs;
  }
  return true;
}

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_carrl_interface
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  carrl_interface::msg::CurveCoeffs & ros_message)
{
  // Member: degree
  cdr >> ros_message.degree;

  // Member: coeffs
  {
    cdr >> ros_message.coeffs;
  }

  return true;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_carrl_interface
get_serialized_size(
  const carrl_interface::msg::CurveCoeffs & ros_message,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // Member: degree
  {
    size_t item_size = sizeof(ros_message.degree);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // Member: coeffs
  {
    size_t array_size = ros_message.coeffs.size();

    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);
    size_t item_size = sizeof(ros_message.coeffs[0]);
    current_alignment += array_size * item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_carrl_interface
max_serialized_size_CurveCoeffs(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;


  // Member: degree
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint8_t);
  }

  // Member: coeffs
  {
    size_t array_size = 0;
    full_bounded = false;
    current_alignment += padding +
      eprosima::fastcdr::Cdr::alignment(current_alignment, padding);

    current_alignment += array_size * sizeof(uint64_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint64_t));
  }

  return current_alignment - initial_alignment;
}

static bool _CurveCoeffs__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  auto typed_message =
    static_cast<const carrl_interface::msg::CurveCoeffs *>(
    untyped_ros_message);
  return cdr_serialize(*typed_message, cdr);
}

static bool _CurveCoeffs__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  auto typed_message =
    static_cast<carrl_interface::msg::CurveCoeffs *>(
    untyped_ros_message);
  return cdr_deserialize(cdr, *typed_message);
}

static uint32_t _CurveCoeffs__get_serialized_size(
  const void * untyped_ros_message)
{
  auto typed_message =
    static_cast<const carrl_interface::msg::CurveCoeffs *>(
    untyped_ros_message);
  return static_cast<uint32_t>(get_serialized_size(*typed_message, 0));
}

static size_t _CurveCoeffs__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_CurveCoeffs(full_bounded, 0);
}

static message_type_support_callbacks_t _CurveCoeffs__callbacks = {
  "carrl_interface::msg",
  "CurveCoeffs",
  _CurveCoeffs__cdr_serialize,
  _CurveCoeffs__cdr_deserialize,
  _CurveCoeffs__get_serialized_size,
  _CurveCoeffs__max_serialized_size
};

static rosidl_message_type_support_t _CurveCoeffs__handle = {
  rosidl_typesupport_fastrtps_cpp::typesupport_identifier,
  &_CurveCoeffs__callbacks,
  get_message_typesupport_handle_function,
};

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace carrl_interface

namespace rosidl_typesupport_fastrtps_cpp
{

template<>
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_EXPORT_carrl_interface
const rosidl_message_type_support_t *
get_message_type_support_handle<carrl_interface::msg::CurveCoeffs>()
{
  return &carrl_interface::msg::typesupport_fastrtps_cpp::_CurveCoeffs__handle;
}

}  // namespace rosidl_typesupport_fastrtps_cpp

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, carrl_interface, msg, CurveCoeffs)() {
  return &carrl_interface::msg::typesupport_fastrtps_cpp::_CurveCoeffs__handle;
}

#ifdef __cplusplus
}
#endif
