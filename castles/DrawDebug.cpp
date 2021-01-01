//
// Created by Jenny Pruitt on 4/1/19.
//

#include "DrawDebug.h"
using namespace cv;
using namespace std;


void DrawDebug::drawLine(const Line& line, cv::Mat& image, cv::Scalar color, int thickness)
{
    //assume color is blue
    Line topBorder(Point2f(0, 0), Point2f(1, 0));
    Line leftBorder(Point2f(0, 0), Point2f(0, 1));
    Line bottomBorder(Point2f(image.cols, image.rows), Point2f(1, 0));
    Line rightBorder(Point2f(image.cols, image.rows), Point2f(0, 1));

    Point2f p1, p2;
    if (line.computeIntersectionPoint(topBorder, p1) && line.computeIntersectionPoint(bottomBorder, p2))
    {
        cv::line(image, p1, p2, Scalar(255, 0, 0), 2, 8);
    }
    else if (line.computeIntersectionPoint(leftBorder, p1) && line.computeIntersectionPoint(rightBorder, p2))
    {
        cv::line(image, p1, p2, Scalar(255,0,0), 2, 8);
    }
}
void DrawDebug::drawLine(const cv::Point2f start, const cv::Point2f end, cv::Mat& image, cv::Scalar color, int thickness)
{
    cv::line(image, start, end, color, thickness, 8);
}
void DrawDebug::printLines(std::vector<Line>& lines){
    for (auto& line: lines)
    {
        cout<<line;
    }

}

cv::Mat DrawDebug:: drawRectangle(cv::Rect2d bbox, cv::Mat& image){
    Mat output = image.clone();
    rectangle(output, bbox, Scalar(255, 0, 0), 2, 1);
    return output;
}

cv::Mat DrawDebug:: drawBalls(std::vector<cv::KeyPoint> keypoints, cv::Mat& image){
    Mat output = image.clone();
    for (size_t i = 0; i < keypoints.size(); ++i) {
        circle(output, keypoints[i].pt, keypoints[i].size/2, Scalar(255, 0, 255), FILLED);
    }
    return output;
}
cv::Mat DrawDebug::drawLines(std::vector<Line>& lines, cv::Mat& image, cv::Scalar color, int thickness)
{
    Mat tempImage = image.clone();
    for (auto& line: lines)
    {
        cout<<line;
        drawLine(line, tempImage, color, thickness);
    }
    return tempImage;
}
void DrawDebug::displayImage( const cv::Mat& image, int delay){
    imshow("frame", image);
    waitKey(delay);
}
