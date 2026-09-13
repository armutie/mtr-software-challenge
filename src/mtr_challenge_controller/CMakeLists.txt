cmake_minimum_required(VERSION 3.8)
project(mtr_challenge_controller)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(waypoint_controller src/waypoint_controller.cpp)
target_compile_features(waypoint_controller PUBLIC cxx_std_17)
target_include_directories(waypoint_controller PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(waypoint_controller geometry_msgs nav_msgs rclcpp)

install(TARGETS waypoint_controller DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY include/ DESTINATION include)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  ament_add_gtest(test_controller_math test/test_controller_math.cpp)
  target_compile_features(test_controller_math PUBLIC cxx_std_17)
  target_include_directories(test_controller_math PUBLIC include)
endif()

ament_package()

