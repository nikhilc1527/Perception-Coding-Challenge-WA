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

std::pair<double, double> least_squares(std::vector<std::pair<double, double>> points) {
  // using formula from https://www.varsitytutors.com/hotmath/hotmath_help/topics/line-of-best-fit
  double mean_x = 0;
  double mean_y = 0;
  for (int i = 0; i < points.size(); i++) {
    mean_x += points[i].first;
    mean_y += points[i].second;
  }
  mean_x /= points.size();
  mean_y /= points.size();
  double top = 0;
  double bottom = 0;
  for (int i = 0; i < points.size(); i++) {
    top += (points[i].first - mean_x) * (points[i].second - mean_y);
    bottom += (points[i].first - mean_x) * (points[i].first - mean_x);
  }
  double m = top / bottom;
  double b = mean_y - m * mean_x;
  return {b, m};
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
  medianBlur(red_img, blurred, 19);

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

  std::vector<std::pair<double, int>> ratios;
  for (int i = 1; i < points.size() - 1; ++i) {
    auto slope1 = (points[i].second - points[i-1].second) / (points[i].first - points[i-1].first);
    auto slope2 = (points[i+1].second - points[i].second) / (points[i+1].first - points[i].first);
    ratios.emplace_back(slope2 / slope1, i);
  }
  for (auto i : ratios) std::cout << i.first << "\n";

  std::sort(ratios.begin(), ratios.end());

  std::cout << ratios[0].first << "\n";
  std::cout << ratios[0].second << "\n";
  std::cout << ratios[ratios.size() - 1].first << "\n";
  std::cout << ratios[ratios.size() - 1].second << "\n";

  std::vector<std::pair<double, double>> left_side, right_side;
  for (int i = 0; i <= ratios[0].second; i++) {
    left_side.push_back(points[i]);
  }
  for (int i = ratios[ratios.size()-1].second; i < points.size(); i++) {
    right_side.push_back(points[i]);
  }

  std::cout << '\n';
  for (auto i : left_side) {
    std::cout << i.first << ", " << i.second << '\n';
  }
  std::cout << '\n';
  for (auto i : right_side) {
    std::cout << i.first << ", " << i.second << '\n';
  }

  // for (int i = 1; i < left_side.size(); i++) {
  //   line(final_img, Point(left_side[i-1].first, left_side[i-1].second), Point(left_side[i].first, left_side[i].second), Scalar(0, 255, 0), 5);    
  // }
  // for (int i = 1; i < right_side.size(); i++) {
  //   line(final_img, Point(right_side[i-1].first, right_side[i-1].second), Point(right_side[i].first, right_side[i].second), Scalar(0, 255, 0), 5);    
  // }

  auto final_img = imread("static/red.png", IMREAD_COLOR);
  
  auto [left_b, left_m] = least_squares(left_side);
  auto [right_b, right_m] = least_squares(right_side);

  std::cout << left_b << ", " << left_m << ", " << right_b << ", " << right_m << "\n";

  auto pointl1 = Point(0, left_b);
  auto pointl2 = Point(final_img.rows, left_m * final_img.rows + left_b);
  line(final_img, pointl1, pointl2, Scalar(0, 255, 0), 5);
  
  auto pointr1 = Point(0, right_b);
  auto pointr2 = Point(final_img.rows, right_m * final_img.rows + right_b);
  line(final_img, pointr1, pointr2, Scalar(0, 255, 0), 5);

  show_img(final_img, "final");

  imwrite("static/my_output.png", final_img);
  
  waitKey(0);
  
  return 0;
}
