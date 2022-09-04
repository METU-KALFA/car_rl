// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from msg_interface:msg/Force.idl
// generated code does not contain a copyright notice
#include "msg_interface/msg/detail/force__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


bool
msg_interface__msg__Force__init(msg_interface__msg__Force * msg)
{
  if (!msg) {
    return false;
  }
  // x
  // y
  return true;
}

void
msg_interface__msg__Force__fini(msg_interface__msg__Force * msg)
{
  if (!msg) {
    return;
  }
  // x
  // y
}

msg_interface__msg__Force *
msg_interface__msg__Force__create()
{
  msg_interface__msg__Force * msg = (msg_interface__msg__Force *)malloc(sizeof(msg_interface__msg__Force));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(msg_interface__msg__Force));
  bool success = msg_interface__msg__Force__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
msg_interface__msg__Force__destroy(msg_interface__msg__Force * msg)
{
  if (msg) {
    msg_interface__msg__Force__fini(msg);
  }
  free(msg);
}


bool
msg_interface__msg__Force__Sequence__init(msg_interface__msg__Force__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  msg_interface__msg__Force * data = NULL;
  if (size) {
    data = (msg_interface__msg__Force *)calloc(size, sizeof(msg_interface__msg__Force));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = msg_interface__msg__Force__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        msg_interface__msg__Force__fini(&data[i - 1]);
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
msg_interface__msg__Force__Sequence__fini(msg_interface__msg__Force__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      msg_interface__msg__Force__fini(&array->data[i]);
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

msg_interface__msg__Force__Sequence *
msg_interface__msg__Force__Sequence__create(size_t size)
{
  msg_interface__msg__Force__Sequence * array = (msg_interface__msg__Force__Sequence *)malloc(sizeof(msg_interface__msg__Force__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = msg_interface__msg__Force__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
msg_interface__msg__Force__Sequence__destroy(msg_interface__msg__Force__Sequence * array)
{
  if (array) {
    msg_interface__msg__Force__Sequence__fini(array);
  }
  free(array);
}
