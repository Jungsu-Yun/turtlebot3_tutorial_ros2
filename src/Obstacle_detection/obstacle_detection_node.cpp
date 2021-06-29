#include "ObstacleDetection.h"

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ObstacleDetection>(false, 0.3));
    rclcpp::shutdown();

    return 0;
}