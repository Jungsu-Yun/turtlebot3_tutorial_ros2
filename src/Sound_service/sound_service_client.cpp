#include "SoundClient.h"
#include <unistd.h>

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto sound_client = std::make_shared<SoundClient>();

    while(true)
    {
        std::string input_data;

        std::system("clear");
        sound_client->print_command_data();
        std::cout << "please insert command : ";
        std::cin >> input_data;

        if(input_data == "q")
            break;
        else
        {
            sound_client->call_service(input_data);
            sleep(5);
        }
    }

    rclcpp::shutdown();

    return 0;
}