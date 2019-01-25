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


public:
  std::vector<std::string> tokens;
};


int main (int argc, char** argv) {

  ArgParser args(argc, argv);

  std::string file_name = "/home/harsh/dataset/dji_flightlogs/DJI_0015.MP4";
  std::string save_path = "/home/harsh/dataset/dji_flightlogs/snaps/";
  bool display = false;

  auto pos = find(args.tokens.begin(), args.tokens.end(), "--help");
  if(pos != args.tokens.end()) {
    std::cout << "--file <video file path>\n--display : To open video display\n--savepath <image snapshot save path>" << std::endl;

    return 0;
  }

  pos = find(args.tokens.begin(), args.tokens.end(), "--file");
  if(pos != args.tokens.end())
    file_name = *(pos + 1);

  pos = find(args.tokens.begin(), args.tokens.end(), "--savepath");
  if(pos != args.tokens.end())
    save_path = *(pos + 1);

  pos = find(args.tokens.begin(), args.tokens.end(), "--display");
  if(pos != args.tokens.end())
    display = true;

  ros::init (argc, argv, "video_publisher");
  ros::NodeHandle nh;

  ros::Publisher pub = nh.advertise<sensor_msgs::Image>("video", 1);

  cv_bridge::CvImage cv_image;
  sensor_msgs::Image ros_image;

  cv::VideoCapture video(file_name);
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

    if(display) {
      cv::imshow("Video Stream", frame);
      char keyInput = cv::waitKey(1);
      if(keyInput == 's') {
        std::cout << "Saving snapshot" << std::endl;
        cv::imwrite(save_path + "frame" + std::to_string(i) + ".png", frame);
      }
      else if(keyInput == ' ')
        while(true) {
          std::cout << "Stream Paused!, Press Space to unpause." << std::endl;
          keyInput = cv::waitKey(1);
          if(keyInput == 's') {
            std::cout << "Saving snapshot" << std::endl;
            cv::imwrite(save_path + "frame" + std::to_string(i) + ".png", frame);
          }
          if(keyInput == ' ') {
            std::cout << "Unpausing Stream" << std::endl;
            break;
          }

          r.sleep();
        }
    }

    r.sleep();
  }

  return 0;
}
