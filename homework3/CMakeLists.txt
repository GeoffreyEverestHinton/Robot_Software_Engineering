cmake_minimum_required(VERSION 3.8)
project(homework3)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/NumStr.msg"
  DEPENDENCIES std_msgs
)

ament_export_dependencies(rosidl_default_runtime)

add_executable(publisher publisher.cpp)
ament_target_dependencies(publisher rclcpp std_msgs)
rosidl_get_typesupport_target(cpp_typesupport_target ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(publisher "${cpp_typesupport_target}")

install(TARGETS
  publisher
  DESTINATION lib/${PROJECT_NAME}
)

install(PROGRAMS
  subscriber.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
