#include "Teleoperation.h"

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto teleoperation_node = std::make_shared<Teleoperation>();
    teleoperation_node->teleoperation_node();
    rclcpp::shutdown();
    return 0;
}