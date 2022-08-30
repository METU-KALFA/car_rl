// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice

#ifndef CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__STRUCT_H_
#define CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'coeffs'
#include "rosidl_runtime_c/primitives_sequence.h"

// Struct defined in msg/CurveCoeffs in the package carrl_interface.
typedef struct carrl_interface__msg__CurveCoeffs
{
  uint8_t degree;
  rosidl_runtime_c__double__Sequence coeffs;
} carrl_interface__msg__CurveCoeffs;

// Struct for a sequence of carrl_interface__msg__CurveCoeffs.
typedef struct carrl_interface__msg__CurveCoeffs__Sequence
{
  carrl_interface__msg__CurveCoeffs * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} carrl_interface__msg__CurveCoeffs__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__STRUCT_H_
