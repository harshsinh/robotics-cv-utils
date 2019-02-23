#include <sstream>
#include <ros/ros.h>
#include <std_msgs/Int16.h>
#include "CVInclude.h"

#define fps 300

using namespace std;

cv::VideoCapture cap;
cv::Mat image, gray;
image_transport::Publisher impub;

void msgCallback (const sensor_msgs::ImageConstPtr& msg)
{
    image = cv_bridge::toCvShare (msg) -> image;

    if (!image.empty ()) {
        cv::cvtColor (image, gray, CV_BGR2GRAY);

        std::cout << image.size << std::endl;

//        cv::imshow("gray", gray);
//        cv::waitKey(0);

        auto detect_msg = cv_bridge::CvImage (std_msgs::Header(), "mono8", gray).toImageMsg();
        impub.publish(detect_msg);
        return;
    }
    ROS_ERROR_STREAM ("Camera not open!");
    return;
}

int main (int argc, char* argv [])
{
    ros::init (argc, argv, "gray_driver");
    ros::NodeHandle nh;
    image_transport::ImageTransport it (nh);

//    cv::namedWindow ("gray");
//    cv::startWindowThread ();

    image_transport::Subscriber sub = it.subscribe ("image_raw", 1000, msgCallback);
    impub= it.advertise ("/mask", 10);

    while(ros::ok()) {
        ros::spinOnce();
        ros::Rate loop_rate (fps);
    }


    return 0;
}
