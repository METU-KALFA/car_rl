// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from service_interface:srv/VelocityBoundaryCheck.idl
// generated code does not contain a copyright notice

#ifndef SERVICE_INTERFACE__SRV__DETAIL__VELOCITY_BOUNDARY_CHECK__STRUCT_H_
#define SERVICE_INTERFACE__SRV__DETAIL__VELOCITY_BOUNDARY_CHECK__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Struct defined in srv/VelocityBoundaryCheck in the package service_interface.
typedef struct service_interface__srv__VelocityBoundaryCheck_Request
{
  float x;
  float y;
} service_interface__srv__VelocityBoundaryCheck_Request;

// Struct for a sequence of service_interface__srv__VelocityBoundaryCheck_Request.
typedef struct service_interface__srv__VelocityBoundaryCheck_Request__Sequence
{
  service_interface__srv__VelocityBoundaryCheck_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} service_interface__srv__VelocityBoundaryCheck_Request__Sequence;


// Constants defined in the message

// Struct defined in srv/VelocityBoundaryCheck in the package service_interface.
typedef struct service_interface__srv__VelocityBoundaryCheck_Response
{
  float x;
  float y;
} service_interface__srv__VelocityBoundaryCheck_Response;

// Struct for a sequence of service_interface__srv__VelocityBoundaryCheck_Response.
typedef struct service_interface__srv__VelocityBoundaryCheck_Response__Sequence
{
  service_interface__srv__VelocityBoundaryCheck_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} service_interface__srv__VelocityBoundaryCheck_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SERVICE_INTERFACE__SRV__DETAIL__VELOCITY_BOUNDARY_CHECK__STRUCT_H_
