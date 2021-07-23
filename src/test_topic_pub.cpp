#include <ros/ros.h>

#include <stdio.h>

#include "test_message/TopicTest.h"

std::string _node_name;

int main(int argc, char** argv) {
    // rosrun으로 실행할 경우 noname이 되며
    // roslaunch으로 실행할 경우 roslaunch의 name으로 됨 
    ros::init(argc, argv, "noname");

    _node_name = ros::this_node::getName();
    ROS_INFO("node_name: %s", _node_name.c_str());

    ros::NodeHandle nh;
    int count = 0;
    int control_hz = 100;
    ros::Rate loop_hz(control_hz);

    // topic test start
    printf("--- topic test[%s] test start ---\n", _node_name.c_str());

    ros::Publisher topic_pub = nh.advertise<test_message::TopicTest>("test_topic", 1000);

    while (ros::ok()) {
        ros::Time time = ros::Time::now();
        test_message::TopicTest topicTest;

        topicTest.header.stamp = time;
        topicTest.data = count++;
        topic_pub.publish(topicTest);

        printf("[%s], ts: %lf, send_data: %d\n", _node_name.c_str(), time.toSec(), count);

        ros::spinOnce();
        loop_hz.sleep();
    }

    printf("--- topic test[%s] test end ---\n", _node_name.c_str());
    // topic test end

    return 0;
}