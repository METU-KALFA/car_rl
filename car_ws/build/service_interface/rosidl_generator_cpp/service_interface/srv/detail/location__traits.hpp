// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from service_interface:srv/Location.idl
// generated code does not contain a copyright notice

#ifndef SERVICE_INTERFACE__SRV__DETAIL__LOCATION__TRAITS_HPP_
#define SERVICE_INTERFACE__SRV__DETAIL__LOCATION__TRAITS_HPP_

#include "service_interface/srv/detail/location__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<service_interface::srv::Location_Request>()
{
  return "service_interface::srv::Location_Request";
}

template<>
inline const char * name<service_interface::srv::Location_Request>()
{
  return "service_interface/srv/Location_Request";
}

template<>
struct has_fixed_size<service_interface::srv::Location_Request>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<service_interface::srv::Location_Request>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<service_interface::srv::Location_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<service_interface::srv::Location_Response>()
{
  return "service_interface::srv::Location_Response";
}

template<>
inline const char * name<service_interface::srv::Location_Response>()
{
  return "service_interface/srv/Location_Response";
}

template<>
struct has_fixed_size<service_interface::srv::Location_Response>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<service_interface::srv::Location_Response>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<service_interface::srv::Location_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<service_interface::srv::Location>()
{
  return "service_interface::srv::Location";
}

template<>
inline const char * name<service_interface::srv::Location>()
{
  return "service_interface/srv/Location";
}

template<>
struct has_fixed_size<service_interface::srv::Location>
  : std::integral_constant<
    bool,
    has_fixed_size<service_interface::srv::Location_Request>::value &&
    has_fixed_size<service_interface::srv::Location_Response>::value
  >
{
};

template<>
struct has_bounded_size<service_interface::srv::Location>
  : std::integral_constant<
    bool,
    has_bounded_size<service_interface::srv::Location_Request>::value &&
    has_bounded_size<service_interface::srv::Location_Response>::value
  >
{
};

template<>
struct is_service<service_interface::srv::Location>
  : std::true_type
{
};

template<>
struct is_service_request<service_interface::srv::Location_Request>
  : std::true_type
{
};

template<>
struct is_service_response<service_interface::srv::Location_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // SERVICE_INTERFACE__SRV__DETAIL__LOCATION__TRAITS_HPP_
