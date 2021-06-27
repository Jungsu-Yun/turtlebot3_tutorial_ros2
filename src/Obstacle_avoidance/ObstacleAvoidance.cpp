#include "ObstacleAvoidance.h"

ObstacleAvoidance::ObstacleAvoidance() : Node("turtlebot3_obstacle_avoidance")
{
    this->distance_limit = 0.5;
    auto defalut_qos = rclcpp::QoS(rclcpp::SensorDataQoS());

    this->laser_subscriber = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan", defalut_qos, std::bind(&ObstacleAvoidance::laser_scan_callback, this, _1));
    this->twist_publisher = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    this->sound_node = rclcpp::Node::make_shared("sound_client");
    this->sound_client = sound_node->create_client<turtlebot3_msgs::srv::Sound>("sound");
    RCLCPP_INFO(this->get_logger(), "Obstacle Avoidance!!");
}

void ObstacleAvoidance::laser_scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    if(msg->ranges[0] > this->distance_limit && msg->ranges[45] > this->distance_limit && msg->ranges[315] > this->distance_limit)
    {
        this->twist.linear.x = 0.05;
        this->twist.angular.z = 0;
        RCLCPP_INFO(this->get_logger(), "current distance: left_side = %f \t right_side %f", msg->ranges[345], msg->ranges[315]);
    }
    else
    {
        this->twist.linear.x = 0;
        this->twist.angular.z = 0.5;

        auto sound = std::make_shared<turtlebot3_msgs::srv::Sound::Request>();
        sound->value = 3;

        auto result = sound_client->async_send_request(sound);
        if(rclcpp::spin_until_future_complete(this->sound_node, result) == rclcpp::executor::FutureReturnCode::SUCCESS)
            RCLCPP_WARN(this->get_logger(), "Too close!! turn left side");
        else
            RCLCPP_ERROR(this->get_logger(), "Failed to call service Sound");
    }
    this->twist_publisher->publish(this->twist);
}

ObstacleAvoidance::~ObstacleAvoidance()
{
    this->twist.linear.x = 0;
    this->twist.angular.z = 0;

    this->twist_publisher->publish(this->twist);
}
