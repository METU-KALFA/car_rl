cmake_minimum_required(VERSION 3.5)
project(tan)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -g)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(mavros_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(carrl_interface REQUIRED)

add_executable(go_to_pos src/go_to_pos.cpp)
add_executable(vel_commander src/vel_commander.cpp)
add_executable(mpc_commander src/mpc_commander.cpp)
add_executable(mpc_commander_opt src/mpc_commander_opt.cpp)
add_executable(mpc_commander_opt_curve src/mpc_commander_opt_curve.cpp)
add_executable(tan_commander_curve src/tan_commander_curve.cpp)
add_executable(test_path src/test_path.cpp)
add_executable(test_curve src/test_curve.cpp)

target_link_libraries(mpc_commander_opt /usr/local/lib/libnlopt.so)
target_link_libraries(mpc_commander_opt_curve /usr/local/lib/libnlopt.so)

target_include_directories(go_to_pos PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  go_to_pos
  "rclcpp"
  "std_msgs"
  "geometry_msgs"
  "nav_msgs"
  "mavros_msgs"
)
target_include_directories(vel_commander PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  vel_commander
  "rclcpp"
  "std_msgs"
  "tf2"
  "geometry_msgs"
  "nav_msgs"
  "mavros_msgs"
)
target_include_directories(mpc_commander PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  mpc_commander
  "rclcpp"
  "tf2"
  "std_msgs"
  "geometry_msgs"
  "nav_msgs"
  "mavros_msgs"
)
target_include_directories(mpc_commander_opt PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  mpc_commander_opt
  "rclcpp"
  "tf2"
  "std_msgs"
  "geometry_msgs"
  "nav_msgs"
  "mavros_msgs"
)
target_include_directories(mpc_commander_opt_curve PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  mpc_commander_opt_curve
  "rclcpp"
  "tf2"
  "std_msgs"
  "geometry_msgs"
  "nav_msgs"
  "mavros_msgs"
  "carrl_interface"
)
target_include_directories(tan_commander_curve PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  tan_commander_curve
  "rclcpp"
  "tf2"
  "std_msgs"
  "geometry_msgs"
  "nav_msgs"
  "mavros_msgs"
  "carrl_interface"
)
target_include_directories(test_path PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  test_path
  "rclcpp"
  "std_msgs"
  "geometry_msgs"
  "nav_msgs"
  "mavros_msgs"
)
target_include_directories(test_curve PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  test_curve
  "rclcpp"
  "std_msgs"
  "geometry_msgs"
  "nav_msgs"
  "mavros_msgs"
  "carrl_interface"
)

install(TARGETS go_to_pos vel_commander mpc_commander mpc_commander_opt mpc_commander_opt_curve tan_commander_curve test_path test_curve
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
