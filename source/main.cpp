#include <iostream>
#include <string>

#include <opencv4/opencv2/opencv.hpp>

using namespace cv;

auto main() -> int {
  auto red_img = imread("static/red.png", IMREAD_COLOR);
  namedWindow("input", WINDOW_NORMAL);
  imshow("input", red_img);
  waitKey(0);
  
  return 0;
}
