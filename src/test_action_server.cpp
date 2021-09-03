#include <ros/ros.h>

#include <stdio.h>

#include <actionlib/server/simple_action_server.h>
#include "test_message/ActionTestAction.h"

std::string _node_name;

class TestAction {
    protected:
    ros::NodeHandle nh_;
    actionlib::SimpleActionServer<test_message::ActionTestAction> as_;
    std::string action_name_;
    test_message::ActionTestFeedback feedback_;
    test_message::ActionTestResult result_;

    public:
    TestAction(std::string name) :
        as_(nh_, name, boost::bind(&TestAction::callback, this, _1), false),
        action_name_(name) {
        as_.start();
    }

    ~TestAction() {
    }

    void callback(const test_message::ActionTestGoalConstPtr &goal) {
        int control_hz = 1;
        ros::Rate loop_hz(control_hz);
        bool success = true;

        feedback_.sequence.clear();
        feedback_.sequence.push_back(0);
        feedback_.sequence.push_back(1);

        ROS_INFO("%s: Executing, creating fibonacci sequence of order %i with seeds %i, %i", action_name_.c_str(), goal->order, feedback_.sequence[0], feedback_.sequence[1]);

        for(int i=1; i<=goal->order; i++) {
            if (as_.isPreemptRequested() || !ros::ok()) {
                ROS_INFO("%s: Preempted", action_name_.c_str());
                as_.setPreempted();
                success = false;
                break;
            }
            feedback_.sequence.push_back(feedback_.sequence[i] + feedback_.sequence[i-1]);
            as_.publishFeedback(feedback_);
            loop_hz.sleep();
        }

        if(success) {
            result_.sequence = feedback_.sequence;
            ROS_INFO("%s: Succeeded", action_name_.c_str());
            as_.setSucceeded(result_);
        }
    }
};

int main(int argc, char** argv) {
    // rosrun으로 실행할 경우 actionServerNode이 되며
    // roslaunch으로 실행할 경우 roslaunch의 name으로 됨 
    ros::init(argc, argv, "actionServerNode");

    _node_name = ros::this_node::getName();
    ROS_INFO("node_name: %s", _node_name.c_str());


    // action server test start
    printf("--- action server[%s] test start ---\n", _node_name.c_str());

    TestAction testAction("test_action");
    ros::spin();

    printf("--- action server[%s] test end ---\n", _node_name.c_str());
    // action server test end

    return 0;
}