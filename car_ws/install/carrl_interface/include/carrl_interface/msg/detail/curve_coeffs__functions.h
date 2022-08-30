// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice

#ifndef CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__FUNCTIONS_H_
#define CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "carrl_interface/msg/rosidl_generator_c__visibility_control.h"

#include "carrl_interface/msg/detail/curve_coeffs__struct.h"

/// Initialize msg/CurveCoeffs message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * carrl_interface__msg__CurveCoeffs
 * )) before or use
 * carrl_interface__msg__CurveCoeffs__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_carrl_interface
bool
carrl_interface__msg__CurveCoeffs__init(carrl_interface__msg__CurveCoeffs * msg);

/// Finalize msg/CurveCoeffs message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_carrl_interface
void
carrl_interface__msg__CurveCoeffs__fini(carrl_interface__msg__CurveCoeffs * msg);

/// Create msg/CurveCoeffs message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * carrl_interface__msg__CurveCoeffs__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_carrl_interface
carrl_interface__msg__CurveCoeffs *
carrl_interface__msg__CurveCoeffs__create();

/// Destroy msg/CurveCoeffs message.
/**
 * It calls
 * carrl_interface__msg__CurveCoeffs__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_carrl_interface
void
carrl_interface__msg__CurveCoeffs__destroy(carrl_interface__msg__CurveCoeffs * msg);


/// Initialize array of msg/CurveCoeffs messages.
/**
 * It allocates the memory for the number of elements and calls
 * carrl_interface__msg__CurveCoeffs__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_carrl_interface
bool
carrl_interface__msg__CurveCoeffs__Sequence__init(carrl_interface__msg__CurveCoeffs__Sequence * array, size_t size);

/// Finalize array of msg/CurveCoeffs messages.
/**
 * It calls
 * carrl_interface__msg__CurveCoeffs__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_carrl_interface
void
carrl_interface__msg__CurveCoeffs__Sequence__fini(carrl_interface__msg__CurveCoeffs__Sequence * array);

/// Create array of msg/CurveCoeffs messages.
/**
 * It allocates the memory for the array and calls
 * carrl_interface__msg__CurveCoeffs__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_carrl_interface
carrl_interface__msg__CurveCoeffs__Sequence *
carrl_interface__msg__CurveCoeffs__Sequence__create(size_t size);

/// Destroy array of msg/CurveCoeffs messages.
/**
 * It calls
 * carrl_interface__msg__CurveCoeffs__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_carrl_interface
void
carrl_interface__msg__CurveCoeffs__Sequence__destroy(carrl_interface__msg__CurveCoeffs__Sequence * array);

#ifdef __cplusplus
}
#endif

#endif  // CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__FUNCTIONS_H_
