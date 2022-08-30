// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice

#ifndef CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__TRAITS_HPP_
#define CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__TRAITS_HPP_

#include "carrl_interface/msg/detail/curve_coeffs__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<carrl_interface::msg::CurveCoeffs>()
{
  return "carrl_interface::msg::CurveCoeffs";
}

template<>
inline const char * name<carrl_interface::msg::CurveCoeffs>()
{
  return "carrl_interface/msg/CurveCoeffs";
}

template<>
struct has_fixed_size<carrl_interface::msg::CurveCoeffs>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<carrl_interface::msg::CurveCoeffs>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<carrl_interface::msg::CurveCoeffs>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__TRAITS_HPP_
