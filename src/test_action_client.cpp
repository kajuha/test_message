#include <ros/ros.h>
#include <boost/thread.hpp>

// #include <stdio.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>

#include "test_message/ActionTestAction.h"
#include "test_message/ActionTestResult.h"
#include "test_message/ActionTestFeedback.h"

void spinThread() {
    ros::spin();
}

void doneCallback(const actionlib::SimpleClientGoalState& state, const test_message::ActionTestResultConstPtr& result) {
    ROS_INFO("Finished in state [%s]", state.toString().c_str());
    ROS_INFO("Answer: %i", result->sequence.back());
    ros::shutdown();
}

void activeCallback() {
    ROS_INFO("Goal just went active");
}

void feedbackCallback(const test_message::ActionTestFeedbackConstPtr& feedback) {
    ROS_INFO("Got Feedback of length %lu", feedback->sequence.size());
}

std::string _node_name;

int main(int argc, char** argv) {
    // rosrun으로 실행할 경우 actionClientNode이 되며
    // roslaunch으로 실행할 경우 roslaunch의 name으로 됨 
    ros::init(argc, argv, "actionClientNode");

    _node_name = ros::this_node::getName();
    ROS_INFO("node_name: %s", _node_name.c_str());


    // action client test start
    printf("--- action client[%s] test start ---\n", _node_name.c_str());

    actionlib::SimpleActionClient<test_message::ActionTestAction> ac("test_action", true);
    boost::thread spin_thread(&spinThread);

    ROS_INFO("Waiting for action server to start.");
    ac.waitForServer();
    ROS_INFO("Action server started, sending goal.");

    test_message::ActionTestGoal goal;
    
    goal.order = 20;
    ac.sendGoal(goal, &doneCallback, &activeCallback, &feedbackCallback);

    bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

    if (finished_before_timeout) {
        actionlib::SimpleClientGoalState state = ac.getState();
        ROS_INFO("Action finished: %s", state.toString().c_str());
    } else {
        ROS_INFO("Action did not finish before the time out.");
    }

    test_message::ActionTestResultConstPtr result = ac.getResult();
    // std::cout << typeid(result.get()).name() << std::endl;
    uint64_t size = result->sequence.size();
    printf("size: %ld\n", size);
    #if 0
    copy(result->sequence.begin(), result->sequence.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    #else
    for (int i=0; i<size; i++) {
        std::cout << result->sequence[i] << " ";
    }
    std::cout << std::endl;
    #endif

    ros::shutdown();
    spin_thread.join();

    printf("--- action client[%s] test end ---\n", _node_name.c_str());
    // action client test end

    return 0;
}