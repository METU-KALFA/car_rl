// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from msg_interface:msg/Force.idl
// generated code does not contain a copyright notice

#ifndef MSG_INTERFACE__MSG__DETAIL__FORCE__STRUCT_H_
#define MSG_INTERFACE__MSG__DETAIL__FORCE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in msg/Force in the package msg_interface.
typedef struct msg_interface__msg__Force
{
  float x;
  float y;
} msg_interface__msg__Force;

// Struct for a sequence of msg_interface__msg__Force.
typedef struct msg_interface__msg__Force__Sequence
{
  msg_interface__msg__Force * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} msg_interface__msg__Force__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // MSG_INTERFACE__MSG__DETAIL__FORCE__STRUCT_H_
