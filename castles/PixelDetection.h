//
// Created by Jenny Pruitt on 4/14/19.
//

#ifndef CPP_COURT_LINE_PIXELDETECTION_H
#define CPP_COURT_LINE_PIXELDETECTION_H

#include "opencv2/opencv.hpp"


class PixelDetection {
public:
    void computeStructureTensorElements(const cv::Mat& image, cv::Mat& dx2, cv::Mat& dxy, cv::Mat& dy2);
    cv::Mat filterLinePixels(const cv::Mat& binaryImage, const cv::Mat& luminanceImage);
    uchar isLinePixel(const cv::Mat& image, unsigned int x, unsigned int y);
    cv::Mat luminanceConvert(cv::Mat frame);
    cv::Mat detectPixels(cv::Mat luminanceChannel);
    cv::Mat run(cv::Mat frame);
    cv::Mat detectBallPixels(cv::Mat frame);

private:

};


#endif //CPP_COURT_LINE_PIXELDETECTION_H
