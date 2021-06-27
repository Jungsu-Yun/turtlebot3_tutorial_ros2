#ifndef TURTLEBOT3_EXAMPLE_OBSTACLEDETECTION_H
#define TURTLEBOT3_EXAMPLE_OBSTACLEDETECTION_H

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <turtlebot3_msgs/srv/sound.hpp>
#include <rclcpp/qos.hpp>

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;
using std::placeholders::_1;

class ObstacleDetection : public rclcpp::Node
{
private:
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_subscriber;
    std::shared_ptr<rclcpp::Node> sound_node;
    rclcpp::Client<turtlebot3_msgs::srv::Sound>::SharedPtr sound_client;

    double minimum_distace;

public:
    ObstacleDetection();
    void laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
};


#endif //TURTLEBOT3_EXAMPLE_OBSTACLEDETECTION_H
