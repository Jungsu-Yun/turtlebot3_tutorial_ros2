#ifndef TURTLEBOT3_EXAMPLE_TELEOPERATION_H
#define TURTLEBOT3_EXAMPLE_TELEOPERATION_H

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <geometry_msgs/msg/twist.hpp>

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <cmath>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

#define MAX_TURTLEBOT_LINEAR_SPEED 0.22
#define MAX_TURTLEBOT_ANGULAR_SPEED 2.84

using namespace std::chrono_literals;

class Teleoperation : public rclcpp::Node
{
private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub;
    geometry_msgs::msg::Twist twist;

    double target_twist_linear_x;
    double target_tiwst_angular_z;

    double step_linear_speed;
    double step_angular_speed;

    std::string command;
    int command_cnt;

public:
    Teleoperation();
    void teleoperation_node();
    geometry_msgs::msg::Twist calculate_max_speed(geometry_msgs::msg::Twist twist);
    void publishing_data(geometry_msgs::msg::Twist twist);
    int getch();
    int kbhit();
};


#endif //TURTLEBOT3_EXAMPLE_TELEOPERATION_H
