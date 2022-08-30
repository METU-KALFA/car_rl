// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from service_interface:srv/Location.idl
// generated code does not contain a copyright notice

#ifndef SERVICE_INTERFACE__SRV__DETAIL__LOCATION__FUNCTIONS_H_
#define SERVICE_INTERFACE__SRV__DETAIL__LOCATION__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "service_interface/msg/rosidl_generator_c__visibility_control.h"

#include "service_interface/srv/detail/location__struct.h"

/// Initialize srv/Location message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * service_interface__srv__Location_Request
 * )) before or use
 * service_interface__srv__Location_Request__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
bool
service_interface__srv__Location_Request__init(service_interface__srv__Location_Request * msg);

/// Finalize srv/Location message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
void
service_interface__srv__Location_Request__fini(service_interface__srv__Location_Request * msg);

/// Create srv/Location message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * service_interface__srv__Location_Request__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
service_interface__srv__Location_Request *
service_interface__srv__Location_Request__create();

/// Destroy srv/Location message.
/**
 * It calls
 * service_interface__srv__Location_Request__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
void
service_interface__srv__Location_Request__destroy(service_interface__srv__Location_Request * msg);


/// Initialize array of srv/Location messages.
/**
 * It allocates the memory for the number of elements and calls
 * service_interface__srv__Location_Request__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
bool
service_interface__srv__Location_Request__Sequence__init(service_interface__srv__Location_Request__Sequence * array, size_t size);

/// Finalize array of srv/Location messages.
/**
 * It calls
 * service_interface__srv__Location_Request__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
void
service_interface__srv__Location_Request__Sequence__fini(service_interface__srv__Location_Request__Sequence * array);

/// Create array of srv/Location messages.
/**
 * It allocates the memory for the array and calls
 * service_interface__srv__Location_Request__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
service_interface__srv__Location_Request__Sequence *
service_interface__srv__Location_Request__Sequence__create(size_t size);

/// Destroy array of srv/Location messages.
/**
 * It calls
 * service_interface__srv__Location_Request__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
void
service_interface__srv__Location_Request__Sequence__destroy(service_interface__srv__Location_Request__Sequence * array);

/// Initialize srv/Location message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * service_interface__srv__Location_Response
 * )) before or use
 * service_interface__srv__Location_Response__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
bool
service_interface__srv__Location_Response__init(service_interface__srv__Location_Response * msg);

/// Finalize srv/Location message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
void
service_interface__srv__Location_Response__fini(service_interface__srv__Location_Response * msg);

/// Create srv/Location message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * service_interface__srv__Location_Response__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
service_interface__srv__Location_Response *
service_interface__srv__Location_Response__create();

/// Destroy srv/Location message.
/**
 * It calls
 * service_interface__srv__Location_Response__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
void
service_interface__srv__Location_Response__destroy(service_interface__srv__Location_Response * msg);


/// Initialize array of srv/Location messages.
/**
 * It allocates the memory for the number of elements and calls
 * service_interface__srv__Location_Response__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
bool
service_interface__srv__Location_Response__Sequence__init(service_interface__srv__Location_Response__Sequence * array, size_t size);

/// Finalize array of srv/Location messages.
/**
 * It calls
 * service_interface__srv__Location_Response__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
void
service_interface__srv__Location_Response__Sequence__fini(service_interface__srv__Location_Response__Sequence * array);

/// Create array of srv/Location messages.
/**
 * It allocates the memory for the array and calls
 * service_interface__srv__Location_Response__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
service_interface__srv__Location_Response__Sequence *
service_interface__srv__Location_Response__Sequence__create(size_t size);

/// Destroy array of srv/Location messages.
/**
 * It calls
 * service_interface__srv__Location_Response__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_service_interface
void
service_interface__srv__Location_Response__Sequence__destroy(service_interface__srv__Location_Response__Sequence * array);

#ifdef __cplusplus
}
#endif

#endif  // SERVICE_INTERFACE__SRV__DETAIL__LOCATION__FUNCTIONS_H_
