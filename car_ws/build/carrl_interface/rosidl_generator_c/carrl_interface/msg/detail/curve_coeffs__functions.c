// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice
#include "carrl_interface/msg/detail/curve_coeffs__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


// Include directives for member types
// Member `coeffs`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
carrl_interface__msg__CurveCoeffs__init(carrl_interface__msg__CurveCoeffs * msg)
{
  if (!msg) {
    return false;
  }
  // degree
  // coeffs
  if (!rosidl_runtime_c__double__Sequence__init(&msg->coeffs, 0)) {
    carrl_interface__msg__CurveCoeffs__fini(msg);
    return false;
  }
  return true;
}

void
carrl_interface__msg__CurveCoeffs__fini(carrl_interface__msg__CurveCoeffs * msg)
{
  if (!msg) {
    return;
  }
  // degree
  // coeffs
  rosidl_runtime_c__double__Sequence__fini(&msg->coeffs);
}

carrl_interface__msg__CurveCoeffs *
carrl_interface__msg__CurveCoeffs__create()
{
  carrl_interface__msg__CurveCoeffs * msg = (carrl_interface__msg__CurveCoeffs *)malloc(sizeof(carrl_interface__msg__CurveCoeffs));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(carrl_interface__msg__CurveCoeffs));
  bool success = carrl_interface__msg__CurveCoeffs__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
carrl_interface__msg__CurveCoeffs__destroy(carrl_interface__msg__CurveCoeffs * msg)
{
  if (msg) {
    carrl_interface__msg__CurveCoeffs__fini(msg);
  }
  free(msg);
}


bool
carrl_interface__msg__CurveCoeffs__Sequence__init(carrl_interface__msg__CurveCoeffs__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  carrl_interface__msg__CurveCoeffs * data = NULL;
  if (size) {
    data = (carrl_interface__msg__CurveCoeffs *)calloc(size, sizeof(carrl_interface__msg__CurveCoeffs));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = carrl_interface__msg__CurveCoeffs__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        carrl_interface__msg__CurveCoeffs__fini(&data[i - 1]);
      }
      free(data);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
carrl_interface__msg__CurveCoeffs__Sequence__fini(carrl_interface__msg__CurveCoeffs__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      carrl_interface__msg__CurveCoeffs__fini(&array->data[i]);
    }
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

carrl_interface__msg__CurveCoeffs__Sequence *
carrl_interface__msg__CurveCoeffs__Sequence__create(size_t size)
{
  carrl_interface__msg__CurveCoeffs__Sequence * array = (carrl_interface__msg__CurveCoeffs__Sequence *)malloc(sizeof(carrl_interface__msg__CurveCoeffs__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = carrl_interface__msg__CurveCoeffs__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
carrl_interface__msg__CurveCoeffs__Sequence__destroy(carrl_interface__msg__CurveCoeffs__Sequence * array)
{
  if (array) {
    carrl_interface__msg__CurveCoeffs__Sequence__fini(array);
  }
  free(array);
}
