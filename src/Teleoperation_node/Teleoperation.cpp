#include "Teleoperation.h"

Teleoperation::Teleoperation() : Node("turtlebot3_teleoperation_cpp")
{
    this->cmd_pub = create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    this->target_twist_linear_x = 0;
    this->target_tiwst_angular_z = 0;

    this->step_linear_speed = 0.01;
    this->step_angular_speed = 0.1;
    this->command_cnt = 0;

    this->command = "Let's Control Turtlebot3!\n--------------------------------moving around:\n\tw\na\ts\td\n\tx\nw/x : increase/decrease linear velocity (Burger : ~ 0.22)\na/d : increase/decrease angular velocity (Burger : ~ 2.84)\n\nspace key, s : force stop\n\nq to quit\n";

    teleop = this;
    signal(SIGINT, Teleoperation::interrupt_handler);
}

geometry_msgs::msg::Twist Teleoperation::calculate_max_speed(geometry_msgs::msg::Twist twist)
{
    if(abs(twist.linear.x) > MAX_TURTLEBOT_LINEAR_SPEED)
    {
        if(twist.linear.x < 0)
            twist.linear.x = MAX_TURTLEBOT_LINEAR_SPEED * -1;
        else
            twist.linear.x = MAX_TURTLEBOT_LINEAR_SPEED;
    }
    if(abs(twist.angular.z) > MAX_TURTLEBOT_ANGULAR_SPEED)
    {
        if(twist.angular.z < 0)
            twist.angular.z = MAX_TURTLEBOT_ANGULAR_SPEED * -1;
        else
            twist.angular.z = MAX_TURTLEBOT_ANGULAR_SPEED;
    }
    return twist;
}

void Teleoperation::publishing_data(geometry_msgs::msg::Twist twist)
{
    this->cmd_pub->publish(twist);
}

void Teleoperation::teleoperation_node()
{
    std::cout << command << std::endl;
    while(rclcpp::ok())
    {
        if(kbhit())
        {
            int key = getch();
            if(key == 'w')
                this->target_twist_linear_x += step_linear_speed;
            else if(key == 'x')
                this->target_twist_linear_x -= step_linear_speed;
            else if(key == 'a')
                this->target_tiwst_angular_z += step_angular_speed;
            else if(key == 'd')
                this->target_tiwst_angular_z -= step_angular_speed;
            else if(key == 's' || key ==' ')
            {
                this->target_twist_linear_x = 0;
                this->target_tiwst_angular_z = 0;
            }
            else if(key == 'q')
            {
                this->twist.linear.x = 0;
                this->twist.angular.z = 0;
                publishing_data(this->twist);
                RCLCPP_ERROR(this->get_logger(), "Stop Program");
                break;
            }

            this->twist.linear.x = this->target_twist_linear_x;
            this->twist.angular.z = this->target_tiwst_angular_z;

            this->twist = calculate_max_speed(this->twist);

            std::cout << "currently : \t linear velocity : " << this->twist.linear.x << "\t angular velocity : " << this->twist.angular.z << std::endl;

            this->command_cnt++;
            if(this->command_cnt % 20 == 0)
            {
                std::system("clear");
                std::cout << command << std::endl;
            }
        }
        publishing_data(this->twist);
    }
}

int Teleoperation::getch()
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

int Teleoperation::kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void Teleoperation::stop_turtlebot()
{
    this->twist.linear.x = 0;
    this->twist.angular.z = 0;
    publishing_data(this->twist);
    RCLCPP_ERROR(this->get_logger(), "Keyboard Interrupt, Stop Program");
    exit(0);
}

void Teleoperation::interrupt_handler(int signal)
{
    if(signal == 2)
        teleop->stop_turtlebot();
}
