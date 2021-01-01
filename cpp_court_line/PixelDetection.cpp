//
// Created by Jenny Pruitt on 4/14/19.
//

#include "PixelDetection.h"

using namespace std;
using namespace cv;

#define PI 3.14159265

#define fgValue 255
#define bgValue 0

int const whiteThresholdRange = 100; //for YCbCr line detection
int const whiteThresholdBallLower = 40;
int const whiteThresholdBallUpper = 150;
int const t = 4;
int const differenceThreshold = 8;
int const gradientKernelSize = 3;
int const kernelSize = 41;

void PixelDetection::computeStructureTensorElements(const cv::Mat& image, cv::Mat& dx2, cv::Mat& dxy, cv::Mat& dy2)
{
    Mat floatImage, dx, dy;
    image.convertTo(floatImage, CV_32F);
    GaussianBlur(floatImage, floatImage, Size(5,5), 0);
    Sobel(floatImage, dx, CV_32F, 1, 0, gradientKernelSize);
    Sobel(floatImage, dy, CV_32F, 0, 1, gradientKernelSize);
    multiply(dx, dx, dx2);
    multiply(dx, dy, dxy);
    multiply(dy, dy, dy2);
    Mat kernel = Mat::ones(kernelSize, kernelSize, CV_32F);
    filter2D(dx2, dx2, -1, kernel);
    filter2D(dxy, dxy, -1, kernel);
    filter2D(dy2, dy2, -1, kernel);
}

cv::Mat PixelDetection::filterLinePixels(const cv::Mat& binaryImage, const cv::Mat& luminanceImage) {
    Mat dx2, dxy, dy2;
    computeStructureTensorElements(luminanceImage, dx2, dxy, dy2);
    Mat outputImage(binaryImage.rows, binaryImage.cols, CV_8UC1, Scalar(bgValue));
    for (unsigned int x = 0; x < binaryImage.cols; ++x) {
        for (unsigned int y = 0; y < binaryImage.rows; ++y) {
            uchar value = binaryImage.at<uchar>(y, x);
            if (value == fgValue)
            {
                Mat t(2, 2, CV_32F);
                t.at<float>(0, 0) = dx2.at<float>(y, x);
                t.at<float>(0, 1) = dxy.at<float>(y, x);
                t.at<float>(1, 0) = dxy.at<float>(y, x);
                t.at<float>(1, 1) = dy2.at<float>(y, x);
                Mat l;
                eigen(t, l);
                if (l.at<float>(0, 0) > 4 * l.at<float>(0, 1)) {
                    outputImage.at<uchar>(y, x) = fgValue;
                }
            }
        }
    }
    return outputImage;
}

uchar PixelDetection::isLinePixel(const cv::Mat& image, unsigned int x, unsigned int y)
{
    if(x <= t || x >= image.cols - t)
        return bgValue;
    if(y <= t || y >= image.rows - t)
        return bgValue;
    uchar value = image.at<uchar>(y,x);
    if( value < whiteThresholdRange)
        return bgValue;
    uchar topValue = image.at<uchar>(y-t, x);
    uchar lowerValue = image.at<uchar>(y+t, x);
    uchar leftValue = image.at<uchar>(y,x-t);
    uchar rightValue = image.at<uchar>(y,x+t);
    if ((value - leftValue > differenceThreshold) && (value - rightValue > differenceThreshold))
    {
        return fgValue;
    }

    if ((value - topValue > differenceThreshold) && (value - lowerValue > differenceThreshold))
    {
        return fgValue;
    }

    return bgValue;
}

cv::Mat PixelDetection::luminanceConvert(cv::Mat frame){
    //basically it converts to RGB2YCrCb and makes it so that only the luminance is in the mat
    Mat imgYCbCr;
    cvtColor(frame, imgYCbCr, COLOR_RGB2YCrCb);
    Mat luminanceChannel(frame.rows, frame.cols, CV_8UC1);
    const int from_to[2] = {0, 0};
    mixChannels(&frame, 1, &luminanceChannel, 1, from_to, 1);
    return luminanceChannel;
}

cv::Mat PixelDetection::detectPixels(cv::Mat luminanceChannel){

    Mat pixelImage(luminanceChannel.rows, luminanceChannel.cols, CV_8UC1, Scalar(bgValue));
    for (unsigned int x = 0; x < luminanceChannel.cols; ++x)
    {
        for (unsigned int y = 0; y < luminanceChannel.rows; ++y)
        {
            pixelImage.at<uchar>(y,x) = isLinePixel(luminanceChannel, x, y);
        }
    }
    return pixelImage;
}

cv::Mat PixelDetection::detectBallPixels(cv::Mat frame){//for balls
    //TODO: make this make more sense with names
    Mat luminanceChannel = luminanceConvert(frame);
    Mat pixelImage(luminanceChannel.rows, luminanceChannel.cols, CV_8UC1, Scalar(bgValue));
    for (unsigned int x = 0; x < luminanceChannel.cols; ++x)
    {
        for (unsigned int y = 0; y < luminanceChannel.rows; ++y)
        {
            uchar value = luminanceChannel.at<uchar>(y,x);
            if(  value > whiteThresholdBallUpper || value < whiteThresholdBallLower)
                pixelImage.at<uchar>(y,x) =  bgValue;
            else
                pixelImage.at<uchar>(y,x) = fgValue;
        }
    }
    return pixelImage;
}

cv::Mat PixelDetection::run(cv::Mat frame){ //for lines
    Mat luminanceChannel = luminanceConvert(frame);
    Mat white = detectPixels(luminanceChannel);
    white = filterLinePixels(white, luminanceChannel);
    return white;
}
