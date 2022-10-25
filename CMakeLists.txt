cmake_minimum_required(VERSION 3.8)
project(tf_publisher)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)

add_executable(odom_tf src/odom.cc)
ament_target_dependencies(
  odom_tf
  geometry_msgs
  nav_msgs
  rclcpp
  tf2
  tf2_ros
)

install(
  TARGETS odom_tf
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
