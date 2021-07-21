#include <ros/ros.h>

#include <stdio.h>

std::string _node_name;

int main(int argc, char** argv) {
    // rosrun으로 실행할 경우 noname이 되며
    // roslaunch으로 실행할 경우 roslaunch의 name으로 됨 
    ros::init(argc, argv, "noname");

    _node_name = ros::this_node::getName();
    ROS_INFO("node_name: %s", _node_name.c_str());

    ros::NodeHandle nh;
    int count = 10;
    int control_hz = 1;

    // topic test
    printf("--- topic test ---\n");

    // ros::Publisher topic_pub = nh.advertise<std_msgs::String>("topic_pub", 1000);
    ros::Rate loop_hz(control_hz);

    while (ros::ok()) {
        // std_msgs::String msg;

        // msg.data = "hello world";
        // topic_pub.publish(msg);

        ros::spinOnce();

        loop_hz.sleep();
    }

    // server-client test
    // printf("--- server-client test ---\n");

    // action test
    // printf("--- action test ---\n");

    return 0;
}