// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from msg_interface:msg/Control.idl
// generated code does not contain a copyright notice

#ifndef MSG_INTERFACE__MSG__DETAIL__CONTROL__STRUCT_HPP_
#define MSG_INTERFACE__MSG__DETAIL__CONTROL__STRUCT_HPP_

#include <rosidl_runtime_cpp/bounded_vector.hpp>
#include <rosidl_runtime_cpp/message_initialization.hpp>
#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>


#ifndef _WIN32
# define DEPRECATED__msg_interface__msg__Control __attribute__((deprecated))
#else
# define DEPRECATED__msg_interface__msg__Control __declspec(deprecated)
#endif

namespace msg_interface
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Control_
{
  using Type = Control_<ContainerAllocator>;

  explicit Control_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->vx = 0.0f;
      this->vy = 0.0f;
    }
  }

  explicit Control_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->vx = 0.0f;
      this->vy = 0.0f;
    }
  }

  // field types and members
  using _vx_type =
    float;
  _vx_type vx;
  using _vy_type =
    float;
  _vy_type vy;

  // setters for named parameter idiom
  Type & set__vx(
    const float & _arg)
  {
    this->vx = _arg;
    return *this;
  }
  Type & set__vy(
    const float & _arg)
  {
    this->vy = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    msg_interface::msg::Control_<ContainerAllocator> *;
  using ConstRawPtr =
    const msg_interface::msg::Control_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<msg_interface::msg::Control_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<msg_interface::msg::Control_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      msg_interface::msg::Control_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<msg_interface::msg::Control_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      msg_interface::msg::Control_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<msg_interface::msg::Control_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<msg_interface::msg::Control_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<msg_interface::msg::Control_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__msg_interface__msg__Control
    std::shared_ptr<msg_interface::msg::Control_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__msg_interface__msg__Control
    std::shared_ptr<msg_interface::msg::Control_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Control_ & other) const
  {
    if (this->vx != other.vx) {
      return false;
    }
    if (this->vy != other.vy) {
      return false;
    }
    return true;
  }
  bool operator!=(const Control_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Control_

// alias to use template instance with default allocator
using Control =
  msg_interface::msg::Control_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace msg_interface

#endif  // MSG_INTERFACE__MSG__DETAIL__CONTROL__STRUCT_HPP_
