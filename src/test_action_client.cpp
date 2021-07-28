#include <ros/ros.h>

#include <stdio.h>

#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "test_message/ActionTestAction.h"
#include "test_message/ActionTestResult.h"

std::string _node_name;

int main(int argc, char** argv) {
    // rosrun으로 실행할 경우 noname이 되며
    // roslaunch으로 실행할 경우 roslaunch의 name으로 됨 
    ros::init(argc, argv, "noname");

    _node_name = ros::this_node::getName();
    ROS_INFO("node_name: %s", _node_name.c_str());


    // action client test start
    printf("--- action client[%s] test start ---\n", _node_name.c_str());

    actionlib::SimpleActionClient<test_message::ActionTestAction> ac("test_action", true);
    ROS_INFO("Waiting for action server to start.");
    ac.waitForServer();
    ROS_INFO("Action server started, sending goal.");

    test_message::ActionTestGoal goal;
    
    goal.order = 20;
    ac.sendGoal(goal);

    bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

    if (finished_before_timeout) {
        actionlib::SimpleClientGoalState state = ac.getState();
        ROS_INFO("Action finished: %s", state.toString().c_str());
    } else {
        ROS_INFO("Action did not finish before the time out.");
    }

    test_message::ActionTestResultConstPtr result = ac.getResult();
    
    ROS_INFO("Action result: %d", result.get());

    ros::spin();

    printf("--- action client[%s] test end ---\n", _node_name.c_str());
    // action client test end

    return 0;
}