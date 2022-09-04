// generated from rosidl_typesupport_fastrtps_c/resource/idl__type_support_c.cpp.em
// with input from service_interface:srv/PolyLineIntersectionCheck.idl
// generated code does not contain a copyright notice
#include "service_interface/srv/detail/poly_line_intersection_check__rosidl_typesupport_fastrtps_c.h"


#include <cassert>
#include <limits>
#include <string>
#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "service_interface/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "service_interface/srv/detail/poly_line_intersection_check__struct.h"
#include "service_interface/srv/detail/poly_line_intersection_check__functions.h"
#include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif


// forward declare type support functions


using _PolyLineIntersectionCheck_Request__ros_msg_type = service_interface__srv__PolyLineIntersectionCheck_Request;

static bool _PolyLineIntersectionCheck_Request__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _PolyLineIntersectionCheck_Request__ros_msg_type * ros_message = static_cast<const _PolyLineIntersectionCheck_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: x
  {
    cdr << ros_message->x;
  }

  // Field name: y
  {
    cdr << ros_message->y;
  }

  // Field name: yaw
  {
    cdr << ros_message->yaw;
  }

  return true;
}

static bool _PolyLineIntersectionCheck_Request__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _PolyLineIntersectionCheck_Request__ros_msg_type * ros_message = static_cast<_PolyLineIntersectionCheck_Request__ros_msg_type *>(untyped_ros_message);
  // Field name: x
  {
    cdr >> ros_message->x;
  }

  // Field name: y
  {
    cdr >> ros_message->y;
  }

  // Field name: yaw
  {
    cdr >> ros_message->yaw;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_service_interface
size_t get_serialized_size_service_interface__srv__PolyLineIntersectionCheck_Request(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _PolyLineIntersectionCheck_Request__ros_msg_type * ros_message = static_cast<const _PolyLineIntersectionCheck_Request__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name x
  {
    size_t item_size = sizeof(ros_message->x);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name y
  {
    size_t item_size = sizeof(ros_message->y);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }
  // field.name yaw
  {
    size_t item_size = sizeof(ros_message->yaw);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _PolyLineIntersectionCheck_Request__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_service_interface__srv__PolyLineIntersectionCheck_Request(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_service_interface
size_t max_serialized_size_service_interface__srv__PolyLineIntersectionCheck_Request(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: x
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: y
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }
  // member: yaw
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _PolyLineIntersectionCheck_Request__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_service_interface__srv__PolyLineIntersectionCheck_Request(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_PolyLineIntersectionCheck_Request = {
  "service_interface::srv",
  "PolyLineIntersectionCheck_Request",
  _PolyLineIntersectionCheck_Request__cdr_serialize,
  _PolyLineIntersectionCheck_Request__cdr_deserialize,
  _PolyLineIntersectionCheck_Request__get_serialized_size,
  _PolyLineIntersectionCheck_Request__max_serialized_size
};

static rosidl_message_type_support_t _PolyLineIntersectionCheck_Request__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_PolyLineIntersectionCheck_Request,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, service_interface, srv, PolyLineIntersectionCheck_Request)() {
  return &_PolyLineIntersectionCheck_Request__type_support;
}

#if defined(__cplusplus)
}
#endif

// already included above
// #include <cassert>
// already included above
// #include <limits>
// already included above
// #include <string>
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "rosidl_typesupport_fastrtps_c/wstring_conversion.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
// already included above
// #include "service_interface/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
// already included above
// #include "service_interface/srv/detail/poly_line_intersection_check__struct.h"
// already included above
// #include "service_interface/srv/detail/poly_line_intersection_check__functions.h"
// already included above
// #include "fastcdr/Cdr.h"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

// includes and forward declarations of message dependencies and their conversion functions

#if defined(__cplusplus)
extern "C"
{
#endif


// forward declare type support functions


using _PolyLineIntersectionCheck_Response__ros_msg_type = service_interface__srv__PolyLineIntersectionCheck_Response;

static bool _PolyLineIntersectionCheck_Response__cdr_serialize(
  const void * untyped_ros_message,
  eprosima::fastcdr::Cdr & cdr)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  const _PolyLineIntersectionCheck_Response__ros_msg_type * ros_message = static_cast<const _PolyLineIntersectionCheck_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: dist
  {
    cdr << ros_message->dist;
  }

  return true;
}

static bool _PolyLineIntersectionCheck_Response__cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  void * untyped_ros_message)
{
  if (!untyped_ros_message) {
    fprintf(stderr, "ros message handle is null\n");
    return false;
  }
  _PolyLineIntersectionCheck_Response__ros_msg_type * ros_message = static_cast<_PolyLineIntersectionCheck_Response__ros_msg_type *>(untyped_ros_message);
  // Field name: dist
  {
    cdr >> ros_message->dist;
  }

  return true;
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_service_interface
size_t get_serialized_size_service_interface__srv__PolyLineIntersectionCheck_Response(
  const void * untyped_ros_message,
  size_t current_alignment)
{
  const _PolyLineIntersectionCheck_Response__ros_msg_type * ros_message = static_cast<const _PolyLineIntersectionCheck_Response__ros_msg_type *>(untyped_ros_message);
  (void)ros_message;
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;

  // field.name dist
  {
    size_t item_size = sizeof(ros_message->dist);
    current_alignment += item_size +
      eprosima::fastcdr::Cdr::alignment(current_alignment, item_size);
  }

  return current_alignment - initial_alignment;
}

static uint32_t _PolyLineIntersectionCheck_Response__get_serialized_size(const void * untyped_ros_message)
{
  return static_cast<uint32_t>(
    get_serialized_size_service_interface__srv__PolyLineIntersectionCheck_Response(
      untyped_ros_message, 0));
}

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_service_interface
size_t max_serialized_size_service_interface__srv__PolyLineIntersectionCheck_Response(
  bool & full_bounded,
  size_t current_alignment)
{
  size_t initial_alignment = current_alignment;

  const size_t padding = 4;
  const size_t wchar_size = 4;
  (void)padding;
  (void)wchar_size;
  (void)full_bounded;

  // member: dist
  {
    size_t array_size = 1;

    current_alignment += array_size * sizeof(uint32_t) +
      eprosima::fastcdr::Cdr::alignment(current_alignment, sizeof(uint32_t));
  }

  return current_alignment - initial_alignment;
}

static size_t _PolyLineIntersectionCheck_Response__max_serialized_size(bool & full_bounded)
{
  return max_serialized_size_service_interface__srv__PolyLineIntersectionCheck_Response(
    full_bounded, 0);
}


static message_type_support_callbacks_t __callbacks_PolyLineIntersectionCheck_Response = {
  "service_interface::srv",
  "PolyLineIntersectionCheck_Response",
  _PolyLineIntersectionCheck_Response__cdr_serialize,
  _PolyLineIntersectionCheck_Response__cdr_deserialize,
  _PolyLineIntersectionCheck_Response__get_serialized_size,
  _PolyLineIntersectionCheck_Response__max_serialized_size
};

static rosidl_message_type_support_t _PolyLineIntersectionCheck_Response__type_support = {
  rosidl_typesupport_fastrtps_c__identifier,
  &__callbacks_PolyLineIntersectionCheck_Response,
  get_message_typesupport_handle_function,
};

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, service_interface, srv, PolyLineIntersectionCheck_Response)() {
  return &_PolyLineIntersectionCheck_Response__type_support;
}

#if defined(__cplusplus)
}
#endif

#include "rosidl_typesupport_fastrtps_cpp/service_type_support.h"
#include "rosidl_typesupport_cpp/service_type_support.hpp"
// already included above
// #include "rosidl_typesupport_fastrtps_c/identifier.h"
// already included above
// #include "service_interface/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "service_interface/srv/poly_line_intersection_check.h"

#if defined(__cplusplus)
extern "C"
{
#endif

static service_type_support_callbacks_t PolyLineIntersectionCheck__callbacks = {
  "service_interface::srv",
  "PolyLineIntersectionCheck",
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, service_interface, srv, PolyLineIntersectionCheck_Request)(),
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, service_interface, srv, PolyLineIntersectionCheck_Response)(),
};

static rosidl_service_type_support_t PolyLineIntersectionCheck__handle = {
  rosidl_typesupport_fastrtps_c__identifier,
  &PolyLineIntersectionCheck__callbacks,
  get_service_typesupport_handle_function,
};

const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, service_interface, srv, PolyLineIntersectionCheck)() {
  return &PolyLineIntersectionCheck__handle;
}

#if defined(__cplusplus)
}
#endif
