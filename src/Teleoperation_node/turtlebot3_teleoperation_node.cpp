#include "Teleoperation.h"

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Teleoperation>());
    rclcpp::shutdown();

    return 0;
}