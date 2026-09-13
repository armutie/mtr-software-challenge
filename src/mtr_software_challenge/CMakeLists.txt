cmake_minimum_required(VERSION 3.8)
project(mtr_software_challenge)

add_compile_options(-Wall -Wextra -Wpedantic)
set(CMAKE_CXX_STANDARD 17)

find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(visualization_msgs REQUIRED)

add_executable(simulator src/simulator.cpp)
ament_target_dependencies(simulator geometry_msgs nav_msgs rclcpp visualization_msgs)

add_executable(controller src/controller.cpp)
target_include_directories(controller PRIVATE include)
ament_target_dependencies(controller geometry_msgs nav_msgs rclcpp)

install(TARGETS simulator controller DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch config DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_controller_math test/test_controller_math.cpp)
  target_include_directories(test_controller_math PRIVATE include)
endif()

ament_package()

