#include "ObstacleDetection.h"

ObstacleDetection::ObstacleDetection() : Node("turtlebot3_obstacle_detetcion_node")
{
    auto defalut_qos = rclcpp::QoS(rclcpp::SensorDataQoS());
    this->minimum_distace = 0.15;
    this->laser_subscriber = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan",defalut_qos, std::bind(&ObstacleDetection::laser_callback,
                                                                                                      this, _1));
    this->sound_node = rclcpp::Node::make_shared("sound_client");
    this->sound_client = sound_node->create_client<turtlebot3_msgs::srv::Sound>("sound");
}

void ObstacleDetection::laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    if(msg->ranges[0] < this->minimum_distace && msg->ranges[15] && msg->ranges[345])
    {
        auto sound = std::make_shared<turtlebot3_msgs::srv::Sound::Request>();
        sound->value = 3;

        auto result = sound_client->async_send_request(sound);
        if(rclcpp::spin_until_future_complete(this->sound_node, result) == rclcpp::executor::FutureReturnCode::SUCCESS)
            RCLCPP_WARN(this->get_logger(), "Too Close!!");
        else
            RCLCPP_ERROR(this->get_logger(), "Failed to call service Sound");
    }
    else
    {
        RCLCPP_INFO(this->get_logger(), "Current obstacle distance : %f \t %f", msg->ranges[0], msg->ranges[345]);
    }
}