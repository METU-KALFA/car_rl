// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "carrl_interface/msg/detail/curve_coeffs__rosidl_typesupport_introspection_c.h"
#include "carrl_interface/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "carrl_interface/msg/detail/curve_coeffs__functions.h"
#include "carrl_interface/msg/detail/curve_coeffs__struct.h"


// Include directives for member types
// Member `coeffs`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  carrl_interface__msg__CurveCoeffs__init(message_memory);
}

void CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_fini_function(void * message_memory)
{
  carrl_interface__msg__CurveCoeffs__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_message_member_array[2] = {
  {
    "degree",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(carrl_interface__msg__CurveCoeffs, degree),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "coeffs",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(carrl_interface__msg__CurveCoeffs, coeffs),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_message_members = {
  "carrl_interface__msg",  // message namespace
  "CurveCoeffs",  // message name
  2,  // number of fields
  sizeof(carrl_interface__msg__CurveCoeffs),
  CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_message_member_array,  // message members
  CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_init_function,  // function to initialize message memory (memory has to be allocated)
  CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_message_type_support_handle = {
  0,
  &CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_carrl_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, carrl_interface, msg, CurveCoeffs)() {
  if (!CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_message_type_support_handle.typesupport_identifier) {
    CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &CurveCoeffs__rosidl_typesupport_introspection_c__CurveCoeffs_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
