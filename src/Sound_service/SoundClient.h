#ifndef TURTLEBOT3_TUTORIAL_ROS2_SOUNDCLIENT_H
#define TURTLEBOT3_TUTORIAL_ROS2_SOUNDCLIENT_H

#include <rclcpp/rclcpp.hpp>
#include <turtlebot3_msgs/srv/sound.hpp>

#include <chrono>
#include <cstdlib>
#include <memory>
#include <iostream>
#include <signal.h>

using namespace std::chrono_literals;

#define OFF 0
#define ON 1
#define LOW_BATTERY 2
#define ERROR 3
#define BUTTON1 4
#define BUTTON2 5

class SoundClient : public rclcpp::Node
{
private:
    rclcpp::Client<turtlebot3_msgs::srv::Sound>::SharedPtr sound_client;
    std::shared_ptr<rclcpp::Node> sound_node;
    std::string command_data;

public:
    SoundClient();
    bool call_service(std::string command);
    static void interrupt_handler(int signal);
    void print_command_data();
};

static SoundClient* soundclient;

#endif //TURTLEBOT3_TUTORIAL_ROS2_SOUNDCLIENT_H
