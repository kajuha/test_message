#include <ros/ros.h>

#include <stdio.h>

#include "test_message/TopicTest.h"

std::string _node_name;

void topicCallback(const test_message::TopicTest topicTest) {
    ros::Time time = ros::Time::now();
    printf("[%s], ts: %lf, recv_data: %d\n", _node_name.c_str(), time.toSec(), topicTest.data);
}

int main(int argc, char** argv) {
    // rosrun으로 실행할 경우 topicSubNode이 되며
    // roslaunch으로 실행할 경우 roslaunch의 name으로 됨 
    ros::init(argc, argv, "topicSubNode");

    _node_name = ros::this_node::getName();
    ROS_INFO("node_name: %s", _node_name.c_str());

    ros::NodeHandle nh;
    int control_hz = 100;
    ros::Rate loop_hz(control_hz);

    // topic test start
    printf("--- topic[%s] test start ---\n", _node_name.c_str());

    ros::Subscriber topic_sub = nh.subscribe("/test_topic", 1, topicCallback);

    while (ros::ok()) {
        ros::spinOnce();
        loop_hz.sleep();
    }

    printf("--- topic[%s] test start ---\n", _node_name.c_str());
    // topic test end

    return 0;
}