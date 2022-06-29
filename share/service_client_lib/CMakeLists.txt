cmake_minimum_required(VERSION 3.5)
project(service_client_lib)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(service_interface REQUIRED)

include_directories(include/service_client_lib) # to tell CMake to add the directory as -I to the compilation command line

set(HEADER_FILES include/service_client_lib/service_calls.hpp) # you can use just the file you're using but this is mostly used for multiple header files

add_library(service_client_lib src/service_calls.cpp ${HEADER_FILES}) 



ament_target_dependencies(
  service_client_lib
  "rclcpp"
  "service_interface"
)
# this installs the executable in the lib folder to be seen by setup.bash to be seen by ros2 :)
install(TARGETS 
  service_client_lib
  DESTINATION lib/${PROJECT_NAME}/
)

# this line to export the targets, in our case it's a library!
ament_export_targets(service_client_lib HAS_LIBRARY_TARGET)

# install the include/the_library directory to the install/include/the_library
install(
  DIRECTORY include/service_client_lib
  DESTINATION include
)


install(
  TARGETS service_client_lib
  EXPORT service_client_lib
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

ament_package()
