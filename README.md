# turtlebot3_tutorial_ros2
본 리포지토리는 turtlebot3와 ROS2를 활용하여 기본적인 topic publishing, subscribe, service call 등을 실습해보기 위해 존재합니다.

기본적인 Sourcecode는 다음의 페이지를 참고하였습니다.
* [__turtlebot3_example__](https://github.com/ROBOTIS-GIT/turtlebot3/tree/dashing-devel/turtlebot3_example)
* [__ROS2 Dashing Documentation__](https://docs.ros.org/en/dashing/index.html)

본 리포지토리의 프로젝트 개발환경은 다음과 같습니다.
* ROS version : ROS Dashing Diademata
* Language : C++
* Remote computer OS Environment : Ubuntu 18.04 LTS
* Turtlebot version : Turtlebot3 Burger

본 리포지토리에는 다음과 같은 Node가 구성되어 있습니다.
* __teleoperation_node__ : 사용자의 key값을 입력받아 turtlebot을 이동합니다.
* __subscribe_battery_node__ : 현재 Turtlebot의 배터리 잔량을 percentage, voltage로 출력합니다.
* __obstacle_detection_node__ : Teleoperation과 동시에 사용하며, 전방 30º 반경의 laser scan값을 읽어 15cm 이내로 장애물이 감지된 경우 경보 알람을 울리게 합니다.
* __obstacle_avoidance_node__ : turtlebot3가 전진을 하다 전방 45º 반경 내에 30cm 이내로 장애물이 인식되는 경우 시계방향으로 회전하고 장애물이 감지되지 않으면 다시 전진을 합니다.

## 1. 사전 개발환경 세팅
PC Ubuntu 설치 및 turtlebot3를 위한 ros 환경 세팅은 turtlebot3 e-manual을 참고해주시기 바랍니다.

* [Robotis Turtlebot3 E-manual](https://emanual.robotis.com/docs/en/platform/turtlebot3/overview/#overview)

## 2. Teleoperation Node
