#include <bits/stdc++.h>
#include "CVInclude.h"
#include <ros/ros.h>
#include <ros/package.h>
#include <sensor_msgs/image_encodings.h>


class ArgParser {
public:
  ArgParser (int &argc, char **argv) {
    for (int i = 0; i < argc; ++i) {
      this->tokens.push_back(std::string(argv[i]));
    }
  }

  // ArgParser (int &argc, char **argv, std::map<std::string, std::string> names) {
  // }

public:
  std::vector<std::string> tokens;
};


int main (int argc, char** argv) {

  ros::init (argc, argv, "video_publisher");
  ros::NodeHandle nh;

  ros::Publisher pub = nh.advertise<sensor_msgs::Image>("video", 1);

  cv_bridge::CvImage cv_image;
  sensor_msgs::Image ros_image;

  cv::VideoCapture video("/home/harsh/dataset/dji_flightlogs/DJI_0015.MP4");
  cv::Mat frame;
  video >> frame;
  int frame_count = video.get(CV_CAP_PROP_FRAME_COUNT) - 1; //first frame read
  double fps = video.get(CV_CAP_PROP_FPS);
  ros::Rate r(fps);
  for (int i = 0; i < frame_count; ++i) {
    video >> frame;
    cv_image.image = frame;
    cv_image.encoding = "bgr8";
    cv_image.toImageMsg (ros_image);
    pub.publish(ros_image);
    r.sleep();
  }

  return 0;
}
