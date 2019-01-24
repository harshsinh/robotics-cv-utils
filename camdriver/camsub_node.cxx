#include <sstream>
#include <ros/ros.h>
#include <std_msgs/Int16.h>
#include "CVInclude.h"

cv::Mat frame;
image_transport::Publisher pub;

void imCallback (const sensor_msgs::ImageConstPtr& msg) {

    //frame = cv_bridge::toCvShare (msg) -> image;
    pub.publish(msg);
    ROS_WARN("lol");
}

int main (int argc, char ** argv)
{

    ros::init (argc, argv, "camsub_node");
    ros::NodeHandle nh;
    image_transport::ImageTransport it (nh);

    image_transport::Subscriber sub = it.subscribe ("/image_raw", 1, imCallback);
    image_transport::Publisher pub = it.advertise ("image_raw_re", 1);

    ros::Rate loop_rate (30);

    ros::spin();

    return 0;
}
