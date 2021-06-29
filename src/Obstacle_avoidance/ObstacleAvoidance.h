#ifndef TURTLEBOT3_EXAMPLE_OBSTACLEAVOIDANCE_H
#define TURTLEBOT3_EXAMPLE_OBSTACLEAVOIDANCE_H

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <turtlebot3_msgs/srv/sound.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/qos.hpp>

#include <chrono>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <signal.h>

using namespace std::chrono_literals;
using std::placeholders::_1;

class ObstacleAvoidance : public rclcpp::Node
{
private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr twist_publisher;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_subscriber;
    std::shared_ptr<rclcpp::Node> sound_node;
    rclcpp::Client<turtlebot3_msgs::srv::Sound>::SharedPtr sound_client;

    double distance_limit;
    int left_angle;
    int right_angle;
    bool is_real;

    geometry_msgs::msg::Twist twist;

public:
    ObstacleAvoidance(double distance_limit, int left_angle, int right_angle, bool is_real);
    void laser_scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
    void stop_turtlebot();
    static void interrupt_handler(int signal);
};

static ObstacleAvoidance* obstacle_avoidance;

#endif //TURTLEBOT3_EXAMPLE_OBSTACLEAVOIDANCE_H
