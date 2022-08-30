// generated from rosidl_typesupport_c/resource/idl__type_support.cpp.em
// with input from service_interface:srv/ForceBoundaryCheck.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "service_interface/msg/rosidl_typesupport_c__visibility_control.h"
#include "service_interface/srv/detail/force_boundary_check__struct.h"
#include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/message_type_support_dispatch.h"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_c/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace service_interface
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _ForceBoundaryCheck_Request_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ForceBoundaryCheck_Request_type_support_ids_t;

static const _ForceBoundaryCheck_Request_type_support_ids_t _ForceBoundaryCheck_Request_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _ForceBoundaryCheck_Request_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ForceBoundaryCheck_Request_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ForceBoundaryCheck_Request_type_support_symbol_names_t _ForceBoundaryCheck_Request_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, service_interface, srv, ForceBoundaryCheck_Request)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_interface, srv, ForceBoundaryCheck_Request)),
  }
};

typedef struct _ForceBoundaryCheck_Request_type_support_data_t
{
  void * data[2];
} _ForceBoundaryCheck_Request_type_support_data_t;

static _ForceBoundaryCheck_Request_type_support_data_t _ForceBoundaryCheck_Request_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ForceBoundaryCheck_Request_message_typesupport_map = {
  2,
  "service_interface",
  &_ForceBoundaryCheck_Request_message_typesupport_ids.typesupport_identifier[0],
  &_ForceBoundaryCheck_Request_message_typesupport_symbol_names.symbol_name[0],
  &_ForceBoundaryCheck_Request_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ForceBoundaryCheck_Request_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ForceBoundaryCheck_Request_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace service_interface

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_C_EXPORT_service_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, service_interface, srv, ForceBoundaryCheck_Request)() {
  return &::service_interface::srv::rosidl_typesupport_c::ForceBoundaryCheck_Request_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
// already included above
// #include "rosidl_runtime_c/message_type_support_struct.h"
// already included above
// #include "service_interface/msg/rosidl_typesupport_c__visibility_control.h"
// already included above
// #include "service_interface/srv/detail/force_boundary_check__struct.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
// already included above
// #include "rosidl_typesupport_c/message_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_c/visibility_control.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace service_interface
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _ForceBoundaryCheck_Response_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ForceBoundaryCheck_Response_type_support_ids_t;

static const _ForceBoundaryCheck_Response_type_support_ids_t _ForceBoundaryCheck_Response_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _ForceBoundaryCheck_Response_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ForceBoundaryCheck_Response_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ForceBoundaryCheck_Response_type_support_symbol_names_t _ForceBoundaryCheck_Response_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, service_interface, srv, ForceBoundaryCheck_Response)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_interface, srv, ForceBoundaryCheck_Response)),
  }
};

typedef struct _ForceBoundaryCheck_Response_type_support_data_t
{
  void * data[2];
} _ForceBoundaryCheck_Response_type_support_data_t;

static _ForceBoundaryCheck_Response_type_support_data_t _ForceBoundaryCheck_Response_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ForceBoundaryCheck_Response_message_typesupport_map = {
  2,
  "service_interface",
  &_ForceBoundaryCheck_Response_message_typesupport_ids.typesupport_identifier[0],
  &_ForceBoundaryCheck_Response_message_typesupport_symbol_names.symbol_name[0],
  &_ForceBoundaryCheck_Response_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t ForceBoundaryCheck_Response_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ForceBoundaryCheck_Response_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace service_interface

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_C_EXPORT_service_interface
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, service_interface, srv, ForceBoundaryCheck_Response)() {
  return &::service_interface::srv::rosidl_typesupport_c::ForceBoundaryCheck_Response_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif

// already included above
// #include "cstddef"
#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "service_interface/msg/rosidl_typesupport_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/service_type_support_dispatch.h"
// already included above
// #include "rosidl_typesupport_c/type_support_map.h"
// already included above
// #include "rosidl_typesupport_interface/macros.h"

namespace service_interface
{

namespace srv
{

namespace rosidl_typesupport_c
{

typedef struct _ForceBoundaryCheck_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _ForceBoundaryCheck_type_support_ids_t;

static const _ForceBoundaryCheck_type_support_ids_t _ForceBoundaryCheck_service_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _ForceBoundaryCheck_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _ForceBoundaryCheck_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _ForceBoundaryCheck_type_support_symbol_names_t _ForceBoundaryCheck_service_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, service_interface, srv, ForceBoundaryCheck)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_interface, srv, ForceBoundaryCheck)),
  }
};

typedef struct _ForceBoundaryCheck_type_support_data_t
{
  void * data[2];
} _ForceBoundaryCheck_type_support_data_t;

static _ForceBoundaryCheck_type_support_data_t _ForceBoundaryCheck_service_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _ForceBoundaryCheck_service_typesupport_map = {
  2,
  "service_interface",
  &_ForceBoundaryCheck_service_typesupport_ids.typesupport_identifier[0],
  &_ForceBoundaryCheck_service_typesupport_symbol_names.symbol_name[0],
  &_ForceBoundaryCheck_service_typesupport_data.data[0],
};

static const rosidl_service_type_support_t ForceBoundaryCheck_service_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_ForceBoundaryCheck_service_typesupport_map),
  rosidl_typesupport_c__get_service_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_c

}  // namespace srv

}  // namespace service_interface

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_C_EXPORT_service_interface
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_c, service_interface, srv, ForceBoundaryCheck)() {
  return &::service_interface::srv::rosidl_typesupport_c::ForceBoundaryCheck_service_type_support_handle;
}

#ifdef __cplusplus
}
#endif
