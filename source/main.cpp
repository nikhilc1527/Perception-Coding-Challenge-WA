#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <opencv4/opencv2/opencv.hpp>

using namespace cv;

void print_arr(const auto &arr) {
  for (auto i : arr) {
    std::cout << i.first << " " << i.second << '\n';
  }
}

void show_img(const auto &img, const std::string& window_name) {
  namedWindow(window_name, WINDOW_NORMAL);
  // cvtColor(img, img, COLOR_HSV2BGR);
  imshow(window_name, img);
}

auto main() -> int {
  auto red_img = imread("static/red.png", IMREAD_COLOR);
  using Img = decltype(red_img);
  cvtColor(red_img, red_img, COLOR_BGR2HSV);

  Img blurred;
  medianBlur(red_img, blurred, 21);

  auto lower = Mat({0, 200, 150});
  auto higher = Mat({200, 255, 255});
  Img filtered;
  inRange(blurred, lower, higher, filtered);

  auto params = SimpleBlobDetector::Params();
  params.filterByColor = true;
  params.blobColor = 255;
  auto blob_detector = SimpleBlobDetector::create(params);

  Img detected;
  std::vector<KeyPoint> keypoints;
  blob_detector->detect(filtered, keypoints);
  drawKeypoints(filtered, keypoints, detected, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
  
  show_img(red_img, "input");
  show_img(blurred, "blurred");
  show_img(filtered, "filtered");
  show_img(detected, "detected");

  std::vector<std::pair<double, double>> points;
  for (auto i : keypoints) points.emplace_back(i.pt.x, i.pt.y);

  std::sort(points.begin(), points.end());

  print_arr(points);

  std::vector<double> ratios;
  for (int i = 1; i < points.size() - 1; ++i) {
    auto slope1 = (points[i].second - points[i-1].second) / (points[i].first - points[i-1].first);
    auto slope2 = (points[i+1].second - points[i].second) / (points[i+1].first - points[i].first);
    ratios.push_back(slope2 / slope1);
  }
  for (auto i : ratios) std::cout << i << "\n";
  
  waitKey(0);
  
  return 0;
}
