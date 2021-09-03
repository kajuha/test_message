#include <ros/ros.h>

#include <stdio.h>

#include "test_message/ServiceTest.h"

std::string _node_name;

bool serviceCallback(test_message::ServiceTest::Request &req, test_message::ServiceTest::Response &res)
{
    ros::Time time = ros::Time::now();
    res.sum = req.num1 + req.num2;

    printf("[%s srv], ts: %lf, sum: %d\n", _node_name.c_str(), time.toSec(), res.sum);

    return true;
}

int main(int argc, char** argv) {
    // rosrun으로 실행할 경우 srvServerNode이 되며
    // roslaunch으로 실행할 경우 roslaunch의 name으로 됨 
    ros::init(argc, argv, "srvServerNode");

    _node_name = ros::this_node::getName();
    ROS_INFO("node_name: %s", _node_name.c_str());

    ros::NodeHandle nh;
    int control_hz = 100;
    ros::Rate loop_hz(control_hz);

    // service server test start
    printf("--- service server[%s] test start ---\n", _node_name.c_str());

    ros::ServiceServer service_srv = nh.advertiseService("test_service", serviceCallback);

    while (ros::ok()) {
        ros::spinOnce();
        loop_hz.sleep();
    }

    printf("--- service server[%s] test end ---\n", _node_name.c_str());
    // service server test end

    return 0;
}