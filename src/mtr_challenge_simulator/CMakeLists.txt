cmake_minimum_required(VERSION 3.8)
project(mtr_challenge_simulator)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)

add_executable(simulator_node src/simulator_node.cpp)
target_compile_features(simulator_node PUBLIC cxx_std_17)
ament_target_dependencies(
  simulator_node geometry_msgs nav_msgs rclcpp std_msgs visualization_msgs)

install(TARGETS simulator_node DESTINATION lib/${PROJECT_NAME})
ament_package()

