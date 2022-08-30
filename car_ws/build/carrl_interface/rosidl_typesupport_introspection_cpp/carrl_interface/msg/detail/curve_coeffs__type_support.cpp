// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "carrl_interface/msg/detail/curve_coeffs__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace carrl_interface
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void CurveCoeffs_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) carrl_interface::msg::CurveCoeffs(_init);
}

void CurveCoeffs_fini_function(void * message_memory)
{
  auto typed_message = static_cast<carrl_interface::msg::CurveCoeffs *>(message_memory);
  typed_message->~CurveCoeffs();
}

size_t size_function__CurveCoeffs__coeffs(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<double> *>(untyped_member);
  return member->size();
}

const void * get_const_function__CurveCoeffs__coeffs(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<double> *>(untyped_member);
  return &member[index];
}

void * get_function__CurveCoeffs__coeffs(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<double> *>(untyped_member);
  return &member[index];
}

void resize_function__CurveCoeffs__coeffs(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<double> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember CurveCoeffs_message_member_array[2] = {
  {
    "degree",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(carrl_interface::msg::CurveCoeffs, degree),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "coeffs",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(carrl_interface::msg::CurveCoeffs, coeffs),  // bytes offset in struct
    nullptr,  // default value
    size_function__CurveCoeffs__coeffs,  // size() function pointer
    get_const_function__CurveCoeffs__coeffs,  // get_const(index) function pointer
    get_function__CurveCoeffs__coeffs,  // get(index) function pointer
    resize_function__CurveCoeffs__coeffs  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers CurveCoeffs_message_members = {
  "carrl_interface::msg",  // message namespace
  "CurveCoeffs",  // message name
  2,  // number of fields
  sizeof(carrl_interface::msg::CurveCoeffs),
  CurveCoeffs_message_member_array,  // message members
  CurveCoeffs_init_function,  // function to initialize message memory (memory has to be allocated)
  CurveCoeffs_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t CurveCoeffs_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &CurveCoeffs_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace carrl_interface


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<carrl_interface::msg::CurveCoeffs>()
{
  return &::carrl_interface::msg::rosidl_typesupport_introspection_cpp::CurveCoeffs_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, carrl_interface, msg, CurveCoeffs)() {
  return &::carrl_interface::msg::rosidl_typesupport_introspection_cpp::CurveCoeffs_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
