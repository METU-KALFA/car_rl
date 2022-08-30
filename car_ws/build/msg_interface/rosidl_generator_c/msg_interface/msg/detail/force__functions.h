// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from msg_interface:msg/Force.idl
// generated code does not contain a copyright notice

#ifndef MSG_INTERFACE__MSG__DETAIL__FORCE__FUNCTIONS_H_
#define MSG_INTERFACE__MSG__DETAIL__FORCE__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "msg_interface/msg/rosidl_generator_c__visibility_control.h"

#include "msg_interface/msg/detail/force__struct.h"

/// Initialize msg/Force message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * msg_interface__msg__Force
 * )) before or use
 * msg_interface__msg__Force__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_msg_interface
bool
msg_interface__msg__Force__init(msg_interface__msg__Force * msg);

/// Finalize msg/Force message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_msg_interface
void
msg_interface__msg__Force__fini(msg_interface__msg__Force * msg);

/// Create msg/Force message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * msg_interface__msg__Force__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_msg_interface
msg_interface__msg__Force *
msg_interface__msg__Force__create();

/// Destroy msg/Force message.
/**
 * It calls
 * msg_interface__msg__Force__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_msg_interface
void
msg_interface__msg__Force__destroy(msg_interface__msg__Force * msg);


/// Initialize array of msg/Force messages.
/**
 * It allocates the memory for the number of elements and calls
 * msg_interface__msg__Force__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_msg_interface
bool
msg_interface__msg__Force__Sequence__init(msg_interface__msg__Force__Sequence * array, size_t size);

/// Finalize array of msg/Force messages.
/**
 * It calls
 * msg_interface__msg__Force__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_msg_interface
void
msg_interface__msg__Force__Sequence__fini(msg_interface__msg__Force__Sequence * array);

/// Create array of msg/Force messages.
/**
 * It allocates the memory for the array and calls
 * msg_interface__msg__Force__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_msg_interface
msg_interface__msg__Force__Sequence *
msg_interface__msg__Force__Sequence__create(size_t size);

/// Destroy array of msg/Force messages.
/**
 * It calls
 * msg_interface__msg__Force__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_msg_interface
void
msg_interface__msg__Force__Sequence__destroy(msg_interface__msg__Force__Sequence * array);

#ifdef __cplusplus
}
#endif

#endif  // MSG_INTERFACE__MSG__DETAIL__FORCE__FUNCTIONS_H_
