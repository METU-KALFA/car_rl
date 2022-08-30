// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from msg_interface:msg/Control.idl
// generated code does not contain a copyright notice

#ifndef MSG_INTERFACE__MSG__DETAIL__CONTROL__STRUCT_H_
#define MSG_INTERFACE__MSG__DETAIL__CONTROL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/Control in the package msg_interface.
typedef struct msg_interface__msg__Control
{
  float vx;
  float vy;
} msg_interface__msg__Control;

// Struct for a sequence of msg_interface__msg__Control.
typedef struct msg_interface__msg__Control__Sequence
{
  msg_interface__msg__Control * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} msg_interface__msg__Control__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MSG_INTERFACE__MSG__DETAIL__CONTROL__STRUCT_H_
