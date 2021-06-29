#ifndef TURTLEBOT3_EXAMPLE_BATTERYSUBSCRIBER_H
#define TURTLEBOT3_EXAMPLE_BATTERYSUBSCRIBER_H

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/battery_state.hpp>

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;
using std::placeholders::_1;

class BatterySubscriber : public rclcpp::Node
{
private:
    rclcpp::Subscription<sensor_msgs::msg::BatteryState>::SharedPtr battery_subscriber;

public:
    BatterySubscriber();
    void battery_callback(const sensor_msgs::msg::BatteryState::SharedPtr msg) const;
};


#endif //TURTLEBOT3_EXAMPLE_BATTERYSUBSCRIBER_H
