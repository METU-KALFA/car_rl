// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from service_interface:srv/VelocityBoundaryCheck.idl
// generated code does not contain a copyright notice
#include "service_interface/srv/detail/velocity_boundary_check__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool
service_interface__srv__VelocityBoundaryCheck_Request__init(service_interface__srv__VelocityBoundaryCheck_Request * msg)
{
  if (!msg) {
    return false;
  }
  // x
  // y
  return true;
}

void
service_interface__srv__VelocityBoundaryCheck_Request__fini(service_interface__srv__VelocityBoundaryCheck_Request * msg)
{
  if (!msg) {
    return;
  }
  // x
  // y
}

service_interface__srv__VelocityBoundaryCheck_Request *
service_interface__srv__VelocityBoundaryCheck_Request__create()
{
  service_interface__srv__VelocityBoundaryCheck_Request * msg = (service_interface__srv__VelocityBoundaryCheck_Request *)malloc(sizeof(service_interface__srv__VelocityBoundaryCheck_Request));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(service_interface__srv__VelocityBoundaryCheck_Request));
  bool success = service_interface__srv__VelocityBoundaryCheck_Request__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
service_interface__srv__VelocityBoundaryCheck_Request__destroy(service_interface__srv__VelocityBoundaryCheck_Request * msg)
{
  if (msg) {
    service_interface__srv__VelocityBoundaryCheck_Request__fini(msg);
  }
  free(msg);
}


bool
service_interface__srv__VelocityBoundaryCheck_Request__Sequence__init(service_interface__srv__VelocityBoundaryCheck_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  service_interface__srv__VelocityBoundaryCheck_Request * data = NULL;
  if (size) {
    data = (service_interface__srv__VelocityBoundaryCheck_Request *)calloc(size, sizeof(service_interface__srv__VelocityBoundaryCheck_Request));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = service_interface__srv__VelocityBoundaryCheck_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        service_interface__srv__VelocityBoundaryCheck_Request__fini(&data[i - 1]);
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
service_interface__srv__VelocityBoundaryCheck_Request__Sequence__fini(service_interface__srv__VelocityBoundaryCheck_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      service_interface__srv__VelocityBoundaryCheck_Request__fini(&array->data[i]);
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

service_interface__srv__VelocityBoundaryCheck_Request__Sequence *
service_interface__srv__VelocityBoundaryCheck_Request__Sequence__create(size_t size)
{
  service_interface__srv__VelocityBoundaryCheck_Request__Sequence * array = (service_interface__srv__VelocityBoundaryCheck_Request__Sequence *)malloc(sizeof(service_interface__srv__VelocityBoundaryCheck_Request__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = service_interface__srv__VelocityBoundaryCheck_Request__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
service_interface__srv__VelocityBoundaryCheck_Request__Sequence__destroy(service_interface__srv__VelocityBoundaryCheck_Request__Sequence * array)
{
  if (array) {
    service_interface__srv__VelocityBoundaryCheck_Request__Sequence__fini(array);
  }
  free(array);
}


bool
service_interface__srv__VelocityBoundaryCheck_Response__init(service_interface__srv__VelocityBoundaryCheck_Response * msg)
{
  if (!msg) {
    return false;
  }
  // x
  // y
  return true;
}

void
service_interface__srv__VelocityBoundaryCheck_Response__fini(service_interface__srv__VelocityBoundaryCheck_Response * msg)
{
  if (!msg) {
    return;
  }
  // x
  // y
}

service_interface__srv__VelocityBoundaryCheck_Response *
service_interface__srv__VelocityBoundaryCheck_Response__create()
{
  service_interface__srv__VelocityBoundaryCheck_Response * msg = (service_interface__srv__VelocityBoundaryCheck_Response *)malloc(sizeof(service_interface__srv__VelocityBoundaryCheck_Response));
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(service_interface__srv__VelocityBoundaryCheck_Response));
  bool success = service_interface__srv__VelocityBoundaryCheck_Response__init(msg);
  if (!success) {
    free(msg);
    return NULL;
  }
  return msg;
}

void
service_interface__srv__VelocityBoundaryCheck_Response__destroy(service_interface__srv__VelocityBoundaryCheck_Response * msg)
{
  if (msg) {
    service_interface__srv__VelocityBoundaryCheck_Response__fini(msg);
  }
  free(msg);
}


bool
service_interface__srv__VelocityBoundaryCheck_Response__Sequence__init(service_interface__srv__VelocityBoundaryCheck_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  service_interface__srv__VelocityBoundaryCheck_Response * data = NULL;
  if (size) {
    data = (service_interface__srv__VelocityBoundaryCheck_Response *)calloc(size, sizeof(service_interface__srv__VelocityBoundaryCheck_Response));
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = service_interface__srv__VelocityBoundaryCheck_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        service_interface__srv__VelocityBoundaryCheck_Response__fini(&data[i - 1]);
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
service_interface__srv__VelocityBoundaryCheck_Response__Sequence__fini(service_interface__srv__VelocityBoundaryCheck_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      service_interface__srv__VelocityBoundaryCheck_Response__fini(&array->data[i]);
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

service_interface__srv__VelocityBoundaryCheck_Response__Sequence *
service_interface__srv__VelocityBoundaryCheck_Response__Sequence__create(size_t size)
{
  service_interface__srv__VelocityBoundaryCheck_Response__Sequence * array = (service_interface__srv__VelocityBoundaryCheck_Response__Sequence *)malloc(sizeof(service_interface__srv__VelocityBoundaryCheck_Response__Sequence));
  if (!array) {
    return NULL;
  }
  bool success = service_interface__srv__VelocityBoundaryCheck_Response__Sequence__init(array, size);
  if (!success) {
    free(array);
    return NULL;
  }
  return array;
}

void
service_interface__srv__VelocityBoundaryCheck_Response__Sequence__destroy(service_interface__srv__VelocityBoundaryCheck_Response__Sequence * array)
{
  if (array) {
    service_interface__srv__VelocityBoundaryCheck_Response__Sequence__fini(array);
  }
  free(array);
}
