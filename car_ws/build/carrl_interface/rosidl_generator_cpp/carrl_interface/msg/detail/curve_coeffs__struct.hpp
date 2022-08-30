// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from carrl_interface:msg/CurveCoeffs.idl
// generated code does not contain a copyright notice

#ifndef CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__STRUCT_HPP_
#define CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__carrl_interface__msg__CurveCoeffs __attribute__((deprecated))
#else
# define DEPRECATED__carrl_interface__msg__CurveCoeffs __declspec(deprecated)
#endif

namespace carrl_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct CurveCoeffs_
{
  using Type = CurveCoeffs_<ContainerAllocator>;

  explicit CurveCoeffs_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->degree = 0;
    }
  }

  explicit CurveCoeffs_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->degree = 0;
    }
  }

  // field types and members
  using _degree_type =
    uint8_t;
  _degree_type degree;
  using _coeffs_type =
    std::vector<double, typename ContainerAllocator::template rebind<double>::other>;
  _coeffs_type coeffs;

  // setters for named parameter idiom
  Type & set__degree(
    const uint8_t & _arg)
  {
    this->degree = _arg;
    return *this;
  }
  Type & set__coeffs(
    const std::vector<double, typename ContainerAllocator::template rebind<double>::other> & _arg)
  {
    this->coeffs = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    carrl_interface::msg::CurveCoeffs_<ContainerAllocator> *;
  using ConstRawPtr =
    const carrl_interface::msg::CurveCoeffs_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<carrl_interface::msg::CurveCoeffs_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<carrl_interface::msg::CurveCoeffs_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      carrl_interface::msg::CurveCoeffs_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<carrl_interface::msg::CurveCoeffs_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      carrl_interface::msg::CurveCoeffs_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<carrl_interface::msg::CurveCoeffs_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<carrl_interface::msg::CurveCoeffs_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<carrl_interface::msg::CurveCoeffs_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__carrl_interface__msg__CurveCoeffs
    std::shared_ptr<carrl_interface::msg::CurveCoeffs_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__carrl_interface__msg__CurveCoeffs
    std::shared_ptr<carrl_interface::msg::CurveCoeffs_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const CurveCoeffs_ & other) const
  {
    if (this->degree != other.degree) {
      return false;
    }
    if (this->coeffs != other.coeffs) {
      return false;
    }
    return true;
  }
  bool operator!=(const CurveCoeffs_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct CurveCoeffs_

// alias to use template instance with default allocator
using CurveCoeffs =
  carrl_interface::msg::CurveCoeffs_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace carrl_interface

#endif  // CARRL_INTERFACE__MSG__DETAIL__CURVE_COEFFS__STRUCT_HPP_
