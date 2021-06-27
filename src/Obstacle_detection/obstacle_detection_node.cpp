#include "ObstacleDetection.h"

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ObstacleDetection>());
    rclcpp::shutdown();

    return 0;
}