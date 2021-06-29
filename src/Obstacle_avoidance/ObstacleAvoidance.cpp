#include "ObstacleAvoidance.h"

ObstacleAvoidance::ObstacleAvoidance(double distance_limit, int left_angle, int right_angle, bool is_real) : Node("turtlebot3_obstacle_avoidance")
{
    obstacle_avoidance = this;
    this->distance_limit = distance_limit;
    this->left_angle = left_angle;
    this->right_angle = right_angle;
    this->is_real =is_real;

    auto defalut_qos = rclcpp::QoS(rclcpp::SensorDataQoS());

    this->laser_subscriber = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan", defalut_qos, std::bind(&ObstacleAvoidance::laser_scan_callback, this, _1));
    this->twist_publisher = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    this->sound_node = rclcpp::Node::make_shared("sound_client");
    this->sound_client = sound_node->create_client<turtlebot3_msgs::srv::Sound>("sound");
    RCLCPP_INFO(this->get_logger(), "Obstacle Avoidance!!");

    signal(SIGINT, ObstacleAvoidance::interrupt_handler);
}

void ObstacleAvoidance::laser_scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    if(msg->ranges[0] > this->distance_limit && msg->ranges[this->left_angle] > this->distance_limit && msg->ranges[this->right_angle] > this->distance_limit)
    {
        this->twist.linear.x = 0.05;
        this->twist.angular.z = 0;
        RCLCPP_INFO(this->get_logger(), "current distance: left_side = %f \t right_side %f", msg->ranges[this->left_angle], msg->ranges[this->right_angle]);
    }
    else
    {
        this->twist.linear.x = 0;
        this->twist.angular.z = 0.5;

        auto sound = std::make_shared<turtlebot3_msgs::srv::Sound::Request>();
        sound->value = 3;

        if(is_real)
        {
            auto result = sound_client->async_send_request(sound);
            if(rclcpp::spin_until_future_complete(this->sound_node, result) == rclcpp::executor::FutureReturnCode::SUCCESS)
                RCLCPP_WARN(this->get_logger(), "Too close!! turn left side");
            else
                RCLCPP_ERROR(this->get_logger(), "Failed to call service Sound");
        }
        else
            RCLCPP_WARN(this->get_logger(), "Too close!! turn left side");
    }
    this->twist_publisher->publish(this->twist);
}

void ObstacleAvoidance::stop_turtlebot()
{
    this->twist.linear.x = 0;
    this->twist.angular.z = 0;
    this->twist_publisher->publish(this->twist);
    RCLCPP_ERROR(this->get_logger(), "Keyboard Interrupt, Stop Program");
    exit(0);
}

void ObstacleAvoidance::interrupt_handler(int signal)
{
    if(signal == 2)
        obstacle_avoidance->stop_turtlebot();
}
