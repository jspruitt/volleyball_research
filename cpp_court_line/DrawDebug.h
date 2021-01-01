//
// Created by Jenny Pruitt on 4/1/19.
//


#include "opencv2/opencv.hpp"
#include "Line.h"
class DrawDebug{
public:
    static cv::Mat drawBalls(std::vector<cv::KeyPoint> keypoints, cv::Mat& image);
    static cv::Mat drawLines(std::vector<Line>& lines, cv::Mat& image,  cv::Scalar color=cv::Scalar(0,0,255), int thickness=2);
    static void displayImage( const cv::Mat& image, int delay = 0);
    static void drawLine(const Line& line, cv::Mat& image, cv::Scalar color=cv::Scalar(0,0,255), int thickness=2);
    static void drawLine(const cv::Point2f start, const cv::Point2f end, cv::Mat& image, cv::Scalar color=cv::Scalar(0,0,255), int thickness=2);
    static void printLines(std::vector<Line>& lines);
    static cv::Mat drawRectangle(cv::Rect2d bbox, cv::Mat& image);
};


