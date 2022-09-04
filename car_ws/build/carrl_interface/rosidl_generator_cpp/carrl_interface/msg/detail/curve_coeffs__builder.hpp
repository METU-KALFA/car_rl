// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice

#ifndef CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__BUILDER_HPP_
#define CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__BUILDER_HPP_

#include "carrl_interface/msg/detail/curve_coeffs__struct.hpp"
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <utility>


namespace carrl_interface
{

namespace msg
{

namespace builder
{

class Init_CurveCoeffs_coeffs
{
public:
  explicit Init_CurveCoeffs_coeffs(::carrl_interface::msg::CurveCoeffs & msg)
  : msg_(msg)
  {}
  ::carrl_interface::msg::CurveCoeffs coeffs(::carrl_interface::msg::CurveCoeffs::_coeffs_type arg)
  {
    msg_.coeffs = std::move(arg);
    return std::move(msg_);
  }

private:
  ::carrl_interface::msg::CurveCoeffs msg_;
};

class Init_CurveCoeffs_degree
{
public:
  Init_CurveCoeffs_degree()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CurveCoeffs_coeffs degree(::carrl_interface::msg::CurveCoeffs::_degree_type arg)
  {
    msg_.degree = std::move(arg);
    return Init_CurveCoeffs_coeffs(msg_);
  }

private:
  ::carrl_interface::msg::CurveCoeffs msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::carrl_interface::msg::CurveCoeffs>()
{
  return carrl_interface::msg::builder::Init_CurveCoeffs_degree();
}

}  // namespace carrl_interface

#endif  // CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__BUILDER_HPP_
