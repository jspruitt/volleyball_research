//
// Created by Jenny Pruitt on 4/9/19.
//

#ifndef CPP_COURT_LINE_VOLLEYBALLCOURTFITTER_H
#define CPP_COURT_LINE_VOLLEYBALLCOURTFITTER_H

#include "VolleyballCourtModel.h"
#include "Line.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
class VolleyballCourtFitter {
public:
    VolleyballCourtFitter();

    VolleyballCourtModel run(const std::vector<Line>& lines, const cv::Mat& binaryImage, const cv::Mat& rgbImage);

    void getHorizontalAndVerticalLines(const std::vector<Line>& lines, std::vector<Line>& hLines, std::vector<Line>& vLines, const Mat& rgbImage);

    void sortLinesByDistanceToPoint(std::vector<Line>& lines, const cv::Point2f& point);

    void sortVerticalLines(std::vector<Line>& vLines, const cv::Mat& rgbImage);

    void sortHorizontalLines(std::vector<Line>& hLines, const cv::Mat& rgbImage);

    void findBestModelFit(const cv::Mat& binaryImage, const cv::Mat& rgbImage);
private:
    VolleyballCourtModel bestModel;
    std::vector<LinePair> hLinePairs;
    std::vector<LinePair> vLinePairs;
};


#endif //CPP_COURT_LINE_VOLLEYBALLCOURTFITTER_H
