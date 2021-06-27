#include "BatterySubscriber.h"

BatterySubscriber::BatterySubscriber() : Node("turtlebot3_battery_subscriber")
{
    this->battery_subscriber = this->create_subscription<sensor_msgs::msg::BatteryState>("/battery_state", 10, std::bind(&BatterySubscriber::battery_callback, this, _1));
}

void BatterySubscriber::battery_callback(const sensor_msgs::msg::BatteryState::SharedPtr msg) const
{
    double current_voltage = msg->voltage;
    double current_percentage = msg->percentage;

    RCLCPP_INFO(this->get_logger(), "Current Percentage : %f\t Current Voltage : %f", current_percentage, current_voltage);
}