cmake_minimum_required(VERSION 3.5)
project(turtlebot3_tutorial_ros2)

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
find_package(rclpy REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlebot3_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

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

add_executable(teleoperation_node src/Teleoperation_node/turtlebot3_teleoperation_node.cpp src/Teleoperation_node/Teleoperation.h src/Teleoperation_node/Teleoperation.cpp)
ament_target_dependencies(teleoperation_node rclcpp std_msgs geometry_msgs turtlebot3_msgs sensor_msgs)

add_executable(subscribe_battery_node src/Subscribe_Battery/subscribe_battery_node.cpp src/Subscribe_Battery/BatterySubscriber.cpp src/Subscribe_Battery/BatterySubscriber.h)
ament_target_dependencies(subscribe_battery_node rclcpp std_msgs geometry_msgs turtlebot3_msgs sensor_msgs)

add_executable(obstacle_detection_node src/Obstacle_detection/obstacle_detection_node.cpp src/Obstacle_detection/ObstacleDetection.cpp src/Obstacle_detection/ObstacleDetection.h)
ament_target_dependencies(obstacle_detection_node rclcpp std_msgs geometry_msgs turtlebot3_msgs sensor_msgs)

add_executable(obstacle_avoidance_node src/Obstacle_avoidance/obstacle_avoidance_node.cpp src/Obstacle_avoidance/ObstacleAvoidance.cpp src/Obstacle_avoidance/ObstacleAvoidance.h)
ament_target_dependencies(obstacle_avoidance_node rclcpp std_msgs geometry_msgs turtlebot3_msgs sensor_msgs)

add_executable(sound_service_node src/Sound_service/sound_service_client.cpp src/Sound_service/SoundClient.cpp src/Sound_service/SoundClient.h)
ament_target_dependencies(sound_service_node rclcpp std_msgs geometry_msgs turtlebot3_msgs sensor_msgs)

install(TARGETS
        teleoperation_node
        subscribe_battery_node
        obstacle_detection_node
        obstacle_avoidance_node
        sound_service_node
        DESTINATION lib/${PROJECT_NAME})

ament_package()
