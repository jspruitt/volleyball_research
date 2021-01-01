//
// Created by Jenny Pruitt on 4/14/19.
//

#ifndef CPP_COURT_LINE_LINEDETECTION_H
#define CPP_COURT_LINE_LINEDETECTION_H

#include "opencv2/opencv.hpp"
#include "Line.h"


class LineDetection {
private:
    std::vector<Line> houghTransform(cv::Mat white, cv::Mat rgbImage);

    cv::Mat getClosePointsMatrix(Line line, const cv::Mat& binaryImage, const cv::Mat& rgbImage);

    Line getRefinedParameters(Line line, cv::Mat binaryImage, cv::Mat rgbImage);


    void refineLineParameters(std::vector<Line>& lines, const cv::Mat& binaryImage, const cv::Mat& rgbImage);



    void removeDuplicateLines(std::vector<Line>& lines, const cv::Mat& rgbImage);
public:
    std::vector<Line> run(const cv::Mat& binaryImage, const cv::Mat& rgbImage);

};


#endif //CPP_COURT_LINE_LINEDETECTION_H
