#include "ObstacleAvoidance.h"

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ObstacleAvoidance>(0.3, 45, 315, false));
    rclcpp::shutdown();

    return 0;
}