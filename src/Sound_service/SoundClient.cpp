#include "SoundClient.h"

SoundClient::SoundClient() : Node("turtlebot3_sound_controller")
{
    this->sound_client = this->create_client<turtlebot3_msgs::srv::Sound>("sound");
    soundclient = this;
    signal(SIGINT, SoundClient::interrupt_handler);
    this->sound_node = rclcpp::Node::make_shared("sound_client");
    this->command_data = "Let's Control Turtlebot3!\n--------------------------------\ncommand\n--------------------------------\non \t off \t low battery \t error \t error \t button 1 \t button 2\n--------------------------------\non : sound ringing once\noff : turn off sound\nlow battery : Output the low battery sound.\nerror : Output the error sound.\nbutton 1 : Output the button 1 sound.\nbutton 2 : Output the button 2 sound.";
}

bool SoundClient::call_service(std::string command)
{
    auto sound = std::make_shared<turtlebot3_msgs::srv::Sound::Request>();
    if(command == "on")
    {
        RCLCPP_INFO(this->get_logger(), "turn on sound!");
        sound->value = ON;
    }
    else if(command == "off")
    {
        RCLCPP_INFO(this->get_logger(), "turn off sound!");
        sound->value = OFF;
    }
    else if(command == "low battery")
    {
        RCLCPP_INFO(this->get_logger(), "turn on low battery sound!");
        sound->value = LOW_BATTERY;
    }
    else if(command == "error")
    {
        RCLCPP_INFO(this->get_logger(), "turn on error sound!");
        sound->value = ERROR;
    }
    else if(command == "button 1")
    {
        RCLCPP_INFO(this->get_logger(), "turn on button1 sound!");
        sound->value = BUTTON1;
    }
    else if(command == "button 2")
    {
        RCLCPP_INFO(this->get_logger(), "turn on button2 sound!");
        sound->value = BUTTON2;
    }

    while (!sound_client->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
            return false;
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
    }

    auto result = sound_client->async_send_request(sound);

    if (rclcpp::spin_until_future_complete(this->sound_node, result) == rclcpp::executor::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(this->get_logger(), "sound turn off");
        return true;
    }
    else
    {
        RCLCPP_ERROR(this->get_logger(), "Failed to call service Sound");
        return false;
    }
}

void SoundClient::interrupt_handler(int signal)
{
    if(signal == 2)
        soundclient->call_service("off");
    exit(0);
}

void SoundClient::print_command_data()
{
    std::cout << command_data << std::endl;
}
