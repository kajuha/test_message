#include <ros/ros.h>

#include <stdio.h>

#include "test_message/ServiceTest.h"

std::string _node_name;

int main(int argc, char** argv) {
    // rosrun으로 실행할 경우 srvClientNode이 되며
    // roslaunch으로 실행할 경우 roslaunch의 name으로 됨 
    ros::init(argc, argv, "srvClientNode");

    _node_name = ros::this_node::getName();
    ROS_INFO("node_name: %s", _node_name.c_str());

    ros::NodeHandle nh;
    int count = 0;
    int control_hz = 100;
    ros::Rate loop_hz(control_hz);

    // service client test start
    printf("--- service client[%s] test start ---\n", _node_name.c_str());

    ros::ServiceClient client_srv = nh.serviceClient<test_message::ServiceTest>("test_service");

    while (ros::ok()) {
        ros::Time time = ros::Time::now();
        test_message::ServiceTest srv;

        srv.request.num1 = ++count;
        srv.request.num2 = count + 1;

        if (client_srv.call(srv)) {
            printf("[%s srv], ts: %lf, num1: %d, num2: %d\n", _node_name.c_str(), time.toSec(), srv.request.num1, srv.request.num2);
        } else {
            printf("[%s srv], ts: %lf, call failed.\n", _node_name.c_str(), time.toSec());
        }
    }
    
    printf("--- service client[%s] test end ---\n", _node_name.c_str());
    // service client test end

    return 0;
}