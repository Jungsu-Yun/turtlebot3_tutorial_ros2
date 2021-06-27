# turtlebot3_tutorial_ros2
본 리포지토리는 turtlebot3와 ROS2를 활용하여 기본적인 topic publishing, subscribe, service call 등을 실습해보기 위해 존재합니다.

기본적인 Sourcecode는 다음의 페이지를 참고하였습니다.
* [__turtlebot3_example__](https://github.com/ROBOTIS-GIT/turtlebot3/tree/dashing-devel/turtlebot3_example)
* [__ROS2 Dashing Documentation__](https://docs.ros.org/en/dashing/index.html)

본 리포지토리의 프로젝트 개발환경은 다음과 같습니다.
* __ROS version__ : ROS Dashing Diademata
* __Language__ : C++
* __Remote computer OS Environment__ : Ubuntu 18.04 LTS
* __Turtlebot version__ : Turtlebot3 Burger

본 리포지토리에는 다음과 같은 Node가 구성되어 있습니다.
* [__teleoperation_node__](https://github.com/jungsuyun/turtlebot3_tutorial_ros2#2-teleoperation-node) : 사용자의 key값을 입력받아 turtlebot을 이동합니다.
* [__subscribe_battery_node__](https://github.com/jungsuyun/turtlebot3_tutorial_ros2#3-subscribe_battery_node) : 현재 Turtlebot의 배터리 잔량을 percentage, voltage로 출력합니다.
* [__obstacle_detection_node__](https://github.com/jungsuyun/turtlebot3_tutorial_ros2#4-obstacle_detection_node) : Teleoperation과 동시에 사용하며, 전방 30º 반경의 laser scan값을 읽어 15cm 이내로 장애물이 감지된 경우 경보 알람을 울리게 합니다.
* [__obstacle_avoidance_node__](https://github.com/jungsuyun/turtlebot3_tutorial_ros2#5-obstacle_avoidance_node) : turtlebot3가 전진을 하다 전방 45º 반경 내에 30cm 이내로 장애물이 인식되는 경우 시계방향으로 회전하고 장애물이 감지되지 않으면 다시 전진을 합니다.

## 1. 사전 개발환경 세팅
* PC Ubuntu 설치 및 turtlebot3를 위한 ros 환경 세팅은 turtlebot3 e-manual을 참고해주시기 바랍니다.

    * [Robotis Turtlebot3 E-manual](https://emanual.robotis.com/docs/en/platform/turtlebot3/overview/#overview)

## 2. Teleoperation Node
본 노드는 사용자로부터 key를 입력받아 turtlebot의 `cmd_vel` topic에 값을 입력합니다.

### __노드 동작 Process__
<img src="image\teleoperation_process.jpg"/>

### __Sourcecode 설명__
먼저 노드를 `rclcpp::init`을 활용하여 초기화 한 후 무한루프를 돌며 `kbhit()`을 이용하여 사용자가 키보드를 입력했는지 확인합니다.
```c++
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
```

사용자의 키가 입력되었다면 `getch()`을 이용하여 사용자의 key값을 받게 됩니다.
```c++
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
```

사용자의 키값에 따라 각각 속도를 매핑해주도록 합니다.
```c++
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
    break;
else
    RCLCPP_WARN(rclcpp::get_logger("rclcpp"), "Wrong input!");
```
Turtlebot3 Burger의 최대 `Linear 속도는 0.22` 이고 최대 `Angular 각도는 2.84`이다. 해당 속도를 초과하는 값은 입력받을 수 없기 때문에 `calculate_max_speed()`함수를 사용하여 최대 속도 여부를 판단한다.
```c++
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
```
최대속도 검증 후 `geometry_msgs::msg::Twist` 타입의 객체를 `publish`해주도록 한다.
```c++
void Teleoperation::publishing_data(geometry_msgs::msg::Twist twist)
{
    this->cmd_pub->publish(twist);
}
```
### __구동하기__
* Turtlebot 구동 명령어
    ```bash
    $ ros2 launch turtlebot3_bringup robot.launch.py
    ```
* Remoe PC 구동 명령어
    ```bash
    $ ros2 run turtlebot3_tutorial_ros2 teleoperation_node
    ```
* 키가 입력됨에 따라 정상적으로 turtlebot이 이동하는지 확인한다.
### __동작 결과__

## 3. subscribe_battery_node
본 노드는 turtlebot으로부터 `battery_state` topic을 받아 화면에 출력합니다.
### __동작 process__
<img src='image\subscribe_node_process.jpg' />

### __Sourcecode 설명__
BatterySubscriber 생성자에서 `battery_state` topic을 subscribe하는 `rclcpp::Subscription`을 선언한다.
```c++
this->battery_subscriber = this->create_subscription<sensor_msgs::msg::BatteryState>("/battery_state", 10, std::bind(&BatterySubscriber::battery_callback, this, _1));
```

`battery_callback`을 통해 배터리 상태를 지역변수에 담은 후 `RCLCPP_INFO`를 통해 출력한다.
```c++
void BatterySubscriber::battery_callback(const sensor_msgs::msg::BatteryState::SharedPtr msg) const
{
    double current_voltage = msg->voltage;
    double current_percentage = msg->percentage;

    RCLCPP_INFO(this->get_logger(), "Current Percentage : %f\t Current Voltage : %f", current_percentage, current_voltage);
}
```

### __구동하기__
* turtlebot3 구동 명령어
    ```bash
    $ ros2 launch turtlebot3_bringup robot.launch.py
    ```

* Remote PC 구동 명령어
    ```bash
    $ ros2 run turtlebot3_tutorial_ros2 subscribe_battery_node
    ```

### __동작결과__

## 4. obstacle_detection_node
 Teleoperation과 동시에 사용하며, 전방 30º 반경의 laser scan값을 읽어 15cm 이내로 장애물이 감지된 경우 경보 알람을 울리게 합니다.

### __동작 Process__
<img src="image\obstacle_detecion_process.jpg"/>

### __Sourcecode 설명__
`ObstacleDetection`클래스의 Constructor에서 `scan`topic을 subscribe하는 `rclcpp::Subscription`을 선언한다.
```c++
 this->laser_subscriber = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan",defalut_qos, std::bind(&ObstacleDetection::laser_callback,this, _1));
```

`laser callback`함수를 호출하여 `laser_scan`값의 15º값, 0º값, 345º값을 읽어온다. message의 `range`를 사용하며 rage는 각 1도씩 거리정보를 담아두게되는 vector클래스로 구성되어 있다.
```msg
# Single scan from a planar laser range-finder
#
# If you have another ranging device with different behavior (e.g. a sonar
# array), please find or create a different message, since applications
# will make fairly laser-specific assumptions about this data

Header header            # timestamp in the header is the acquisition time of 
                         # the first ray in the scan.
                         #
                         # in frame frame_id, angles are measured around 
                         # the positive Z axis (counterclockwise, if Z is up)
                         # with zero angle being forward along the x axis
                         
float32 angle_min        # start angle of the scan [rad]
float32 angle_max        # end angle of the scan [rad]
float32 angle_increment  # angular distance between measurements [rad]

float32 time_increment   # time between measurements [seconds] - if your scanner
                         # is moving, this will be used in interpolating position
                         # of 3d points
float32 scan_time        # time between scans [seconds]

float32 range_min        # minimum range value [m]
float32 range_max        # maximum range value [m]

float32[] ranges         # range data [m] (Note: values < range_min or > range_max should be discarded)
float32[] intensities    # intensity data [device-specific units].  If your
                         # device does not provide intensities, please leave
                         # the array empty.
```
range값에서 특정 detection 범위 이내로 들어오게 되면 `sound` Service에 Error 값을 request한다.
```c++
if(msg->ranges[0] < this->minimum_distace && msg->ranges[15] && msg->ranges[345])
{
    auto sound = std::make_shared<turtlebot3_msgs::srv::Sound::Request>();
    sound->value = 3;

    auto result = sound_client->async_send_request(sound);
    if(rclcpp::spin_until_future_complete(this->sound_node, result) == rclcpp::executor::FutureReturnCode::SUCCESS)
        RCLCPP_WARN(this->get_logger(), "Too Close!!");
    else
        RCLCPP_ERROR(this->get_logger(), "Failed to call service Sound");
}
```
특정 범위 이외의 값인 경우 현재 측정된 값을 출력한다.
```c++
else
{
    RCLCPP_INFO(this->get_logger(), "Current obstacle distance : %f \t %f", msg->ranges[0], msg->ranges[345]);
}
```

### __구동하기__
* turtlebot3 구동 명령어
    ```bash
    $ ros2 launch turtlebot3_bringup robot.launch.py
    ```

* Remote PC 구동 명령어
    ```bash
    $ ros2 run turtlebot3_tutorial_ros2 obstacle_detection_node
    $ ros2 run turtlebot3_tutorial_ros2 teleoperation_node
    ```

### __동작결과__


## 5. obstacle_avoidance_node
turtlebot3가 전진을 하다 전방 45º 반경 내에 30cm 이내로 장애물이 인식되는 경우 시계방향으로 회전하고 장애물이 감지되지 않으면 다시 전진을 합니다.

### __구동 Process__
<img src="image\obstacle_avoidance_process.jpg"/>

### __Sourcecode 설명__
`ObstacleAvoidance`클래스의 Constructor에서 `scan`topic을 subscribe하는 `rclcpp::Subscription`을 선언한다.
```c++
 this->laser_subscriber = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan",defalut_qos, std::bind(&ObstacleDetection::laser_callback,this, _1));
```

`laser callback`함수를 호출하여 `laser_scan`값의 15º값, 0º값, 345º값을 읽어온다. message의 `range`를 사용하며 rage는 각 1도씩 거리정보를 담아두게되는 vector클래스로 구성되어 있다.
```msg
# Single scan from a planar laser range-finder
#
# If you have another ranging device with different behavior (e.g. a sonar
# array), please find or create a different message, since applications
# will make fairly laser-specific assumptions about this data

Header header            # timestamp in the header is the acquisition time of 
                         # the first ray in the scan.
                         #
                         # in frame frame_id, angles are measured around 
                         # the positive Z axis (counterclockwise, if Z is up)
                         # with zero angle being forward along the x axis
                         
float32 angle_min        # start angle of the scan [rad]
float32 angle_max        # end angle of the scan [rad]
float32 angle_increment  # angular distance between measurements [rad]

float32 time_increment   # time between measurements [seconds] - if your scanner
                         # is moving, this will be used in interpolating position
                         # of 3d points
float32 scan_time        # time between scans [seconds]

float32 range_min        # minimum range value [m]
float32 range_max        # maximum range value [m]

float32[] ranges         # range data [m] (Note: values < range_min or > range_max should be discarded)
float32[] intensities    # intensity data [device-specific units].  If your
                         # device does not provide intensities, please leave
                         # the array empty.
```
range값에서 특정 detection 범위 이내가 아니라면 전진, 범위 이내로 장애물이 인식된 경우 시계방향으로 회전하는 `callback`함수를 구현한다.
```c++
void ObstacleAvoidance::laser_scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    std::cout<< "Callback!!!" << std::endl;
    if(msg->ranges[0] > this->distance_limit && msg->ranges[45] > this->distance_limit && msg->ranges[315] > this->distance_limit)
    {
        this->twist.linear.x = 0.05;
        this->twist.angular.z = 0;
        RCLCPP_INFO(this->get_logger(), "current distance: left_side = %f \t right_side %f", msg->ranges[345], msg->ranges[315]);
    }
    else
    {
        this->twist.linear.x = 0;
        this->twist.angular.z = 0.5;

        auto sound = std::make_shared<turtlebot3_msgs::srv::Sound::Request>();
        sound->value = 3;

        auto result = sound_client->async_send_request(sound);
        if(rclcpp::spin_until_future_complete(this->sound_node, result) == rclcpp::executor::FutureReturnCode::SUCCESS)
            RCLCPP_WARN(this->get_logger(), "Too close!! turn left side");
        else
            RCLCPP_ERROR(this->get_logger(), "Failed to call service Sound");
    }
    this->twist_publisher->publish(this->twist);
}
```

### __구동하기__
* turtlebot3 구동 명령어
    ```bash
    $ ros2 launch turtlebot3_bringup robot.launch.py
    ```

* Remote PC 구동 명령어
    ```bash
    $ ros2 run turtlebot3_tutorial_ros2 obstacle_avoidance_node
    ```

### __동작결과__

