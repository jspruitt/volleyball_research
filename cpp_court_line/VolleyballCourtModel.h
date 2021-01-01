//
// Created by Jenny Pruitt on 4/12/19.
//
#pragma once

#include "Line.h"
#include "opencv2/opencv.hpp"

typedef std::pair<Line, Line> LinePair;

class VolleyballCourtModel
{
public:
    VolleyballCourtModel();

    VolleyballCourtModel(const VolleyballCourtModel& o);

    VolleyballCourtModel& operator=(const VolleyballCourtModel& o);

    float fit(const LinePair& hLinePair, const LinePair& vLinePair, const cv::Mat& binaryImage,
              const cv::Mat& rgbImage);

    static std::vector<LinePair> getPossibleLinePairs(std::vector<Line>& lines);

    void drawModel(cv::Mat& image, cv::Scalar color=cv::Scalar(0, 255, 255));

    void writeToFile(const std::string& filename);

    cv::Mat getTransformationMatrix();

    std::vector<float> getLineDistances();

    void specialPoints(cv::Mat& image, cv::Scalar color=cv::Scalar(0, 255, 255));

    cv::Point2f onePoint(cv::Point2f input);

private:

    float distance(const cv::Point2f& p1, const cv::Point2f& p2);

    cv::Point2f normalize(const cv::Point2f& v);

    std::vector<cv::Point2f> getIntersectionPoints(const LinePair& hLinePair, const LinePair& vLinePair);

    float evaluateModel(const std::vector<cv::Point2f>& courtPoints, const cv::Mat& binaryImage);

    float computeScoreForLineSegment(const cv::Point2f& start, const cv::Point2f& end,
                                     const cv::Mat& binaryImage);

    bool isInsideTheImage(float x, float y, const cv::Mat& image);

    void drawModel(std::vector<cv::Point2f>& courtPoints, cv::Mat& image, cv::Scalar color=cv::Scalar(0, 255, 255));

    std::vector<float> getLineDistances(std::vector<cv::Point2f>& courtPoints);

    std::vector<Line> hLines;
    std::vector<Line> vLines;
    std::vector<LinePair> hLinePairs;
    std::vector<LinePair> vLinePairs;
    std::vector<cv::Point2f> courtPoints;
    cv::Mat transformationMatrix;

};
