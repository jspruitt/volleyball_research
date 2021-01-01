//
// Created by Jenny Pruitt on 4/12/19.
//

#include "VolleyballCourtModel.h"

//
// Created by Chlebus, Grzegorz on 28.08.17.
// Copyright (c) Chlebus, Grzegorz. All rights reserved.
//

#include "VolleyballCourtModel.h"
#include "DrawDebug.h"

#define initialFitScore -1e9
#define fgValue 255

using namespace cv;
using namespace std;
//changed this so that it would work for a volleyball court
VolleyballCourtModel::VolleyballCourtModel()
{
    Point2f hVector(1, 0);
    const Line upperBaseLine = Line(Point2f(0, 0), hVector);
    const Line upperTenFootLine = Line(Point2f(0, 6), hVector);
    const Line netLine = Line(Point2f(0, 9), hVector);
    const Line lowerTenFootLine = Line(Point2f(0, 12), hVector);
    const Line lowerBaseLine = Line(Point2f(0, 18), hVector);
    hLines = {
            upperBaseLine, upperTenFootLine, netLine, lowerTenFootLine, lowerBaseLine
    };

    Point2f vVector(0, 1);
    const Line leftSideLine = Line(Point2f(0, 0), vVector);
    const Line rightSideLine = Line(Point2f(9, 0), vVector);
    vLines = {
            leftSideLine, rightSideLine
    };

    // TODO do not contrain to only these lines
//  hLinePairs = getPossibleLinePairs(hLines);
//  vLinePairs = getPossibleLinePairs(vLines);
    hLinePairs.push_back(std::make_pair(hLines[0], hLines[4]));
    hLinePairs.push_back(std::make_pair(hLines[0], hLines[3]));
    hLinePairs.push_back(std::make_pair(hLines[1], hLines[3]));
    hLinePairs.push_back(std::make_pair(hLines[1], hLines[4]));


    vLinePairs.push_back(std::make_pair(vLines[0], vLines[1]));

    Point2f point;
    if (upperBaseLine.computeIntersectionPoint(leftSideLine, point))
    {
        courtPoints.push_back(point); // P1 this one matters for bd
    }
    if (lowerBaseLine.computeIntersectionPoint(leftSideLine, point))
    {
        courtPoints.push_back(point); // P2 this one matters for bd
    }
    if (lowerBaseLine.computeIntersectionPoint(rightSideLine, point))
    {
        courtPoints.push_back(point); // P3 this one matters for bd
    }
    if (upperBaseLine.computeIntersectionPoint(rightSideLine, point))
    {
        courtPoints.push_back(point);  // P4 this one matters for bd
    }
    if (leftSideLine.computeIntersectionPoint(netLine, point))
    {
        courtPoints.push_back(point);  // P5
    }
    if (rightSideLine.computeIntersectionPoint(netLine, point))
    {
        courtPoints.push_back(point);  // P6
    }
    if (leftSideLine.computeIntersectionPoint(upperTenFootLine, point))
    {
        courtPoints.push_back(point);  // P7
    }
    if (rightSideLine.computeIntersectionPoint(upperTenFootLine, point))
    {
        courtPoints.push_back(point);  // P8
    }
    if (leftSideLine.computeIntersectionPoint(lowerTenFootLine, point))
    {
        courtPoints.push_back(point);  // P9
    }
    if (rightSideLine.computeIntersectionPoint(lowerTenFootLine, point))
    {
        courtPoints.push_back(point);  // P10
    }

    assert(courtPoints.size() == 10);
}

VolleyballCourtModel::VolleyballCourtModel(const VolleyballCourtModel& o)
        : transformationMatrix(o.transformationMatrix)
{
    courtPoints = o.courtPoints;
    hLinePairs = o.hLinePairs;
    vLinePairs = o.vLinePairs;
    hLines = o.hLines;
    vLines = o.vLines;
}

VolleyballCourtModel& VolleyballCourtModel::operator=(const VolleyballCourtModel& o)
{
    transformationMatrix = o.transformationMatrix;
    return *this;
}
cv::Mat VolleyballCourtModel::getTransformationMatrix(){
    return transformationMatrix;
}

std::vector<float> VolleyballCourtModel::getLineDistances()
{
    std::vector<Point2f> transformedModelPoints(10);//changed to 10 from 16
    perspectiveTransform(courtPoints, transformedModelPoints, transformationMatrix);
    return getLineDistances(transformedModelPoints);
}

std::vector<float> VolleyballCourtModel::getLineDistances(std::vector<cv::Point2f>& courtPoints){
    //should be going smaller first then second
    vector<float> d;
    d.push_back(distance(courtPoints[0], courtPoints[2]));
    d.push_back(distance(courtPoints[1], courtPoints[3]));
    return d;
}

float VolleyballCourtModel::fit(const LinePair& hLinePair, const LinePair& vLinePair,
                            const cv::Mat& binaryImage, const cv::Mat& rgbImage)
{
    float bestScore = initialFitScore;
    std::vector<Point2f> points = getIntersectionPoints(hLinePair, vLinePair);
    //TODO Check whether the intersection points make sense

    for (auto& modelHLinePair: hLinePairs)
    {
        for (auto& modelVLinePair: vLinePairs)
        {
            std::vector<Point2f> modelPoints = getIntersectionPoints(modelHLinePair, modelVLinePair);
            Mat matrix = getPerspectiveTransform(modelPoints, points);
            std::vector<Point2f> transformedModelPoints(10);//changed to 10 from 16
            perspectiveTransform(courtPoints, transformedModelPoints, matrix);
            float score = evaluateModel(transformedModelPoints, binaryImage);
            if (score > bestScore)
            {
                bestScore = score;
                transformationMatrix = matrix;
            }
//      Mat image = rgbImage.clone();
//      drawModel(transformedModelPoints, image);
//      displayImage("VolleyballCourtModel", image, 0);
        }
    }
    return bestScore;
}


std::vector<cv::Point2f> VolleyballCourtModel::getIntersectionPoints(const LinePair& hLinePair,
                                                                 const LinePair& vLinePair)
{
    std::vector<Point2f> v;
    Point2f point;

    if (hLinePair.first.computeIntersectionPoint(vLinePair.first, point))
    {
        v.push_back(point);
    }
    if (hLinePair.first.computeIntersectionPoint(vLinePair.second, point))
    {
        v.push_back(point);
    }
    if (hLinePair.second.computeIntersectionPoint(vLinePair.first, point))
    {
        v.push_back(point);
    }
    if (hLinePair.second.computeIntersectionPoint(vLinePair.second, point))
    {
        v.push_back(point);
    }

    assert(v.size() == 4);

    return v;
}

std::vector<LinePair> VolleyballCourtModel::getPossibleLinePairs(std::vector<Line>& lines)
{
    std::vector<LinePair> linePairs;
    for (size_t first = 0; first < lines.size(); ++first)
//  for (size_t first = 0; first < 1; ++first)
    {
        for (size_t second = first + 1; second < lines.size(); ++second)
        {
            linePairs.push_back(std::make_pair(lines[first], lines[second]));
        }
    }
    return linePairs;
}


void VolleyballCourtModel::drawModel(cv::Mat& image, Scalar color)
{
    std::vector<Point2f> transformedModelPoints(10);//changed to 10 from 16
    perspectiveTransform(courtPoints, transformedModelPoints, transformationMatrix);
    drawModel(transformedModelPoints, image, color);
}

cv::Point2f VolleyballCourtModel::onePoint(cv::Point2f input) {
    std::vector<Point2f> transformedModelPoints(1);
    std::vector<Point2f> in;
    in.push_back(input);
    Mat t = transformationMatrix.inv();
    perspectiveTransform(in, transformedModelPoints, t);
    return transformedModelPoints[0];
}
void VolleyballCourtModel::specialPoints(cv::Mat& image, Scalar color){

    vector<Point2f> newPoints;
    newPoints.push_back(Point2f(6, 9));
    newPoints.push_back(Point2f(6, 12));
    newPoints.push_back(Point2f(8, 12));
    newPoints.push_back(Point2f(8, 9));


    std::vector<Point2f> transformedModelPoints(4);
    perspectiveTransform(newPoints, transformedModelPoints, transformationMatrix);

    DrawDebug::drawLine(transformedModelPoints[0], transformedModelPoints[1], image, color);
    DrawDebug::drawLine(transformedModelPoints[1], transformedModelPoints[2], image, color);
    DrawDebug::drawLine(transformedModelPoints[2], transformedModelPoints[3], image, color);
    DrawDebug::drawLine(transformedModelPoints[3], transformedModelPoints[0], image, color);
}

void VolleyballCourtModel::drawModel(std::vector<Point2f>& courtPoints, Mat& image, Scalar color)
{
    DrawDebug::drawLine(courtPoints[0], courtPoints[1], image, color);
    DrawDebug::drawLine(courtPoints[1], courtPoints[2], image, color);
    DrawDebug::drawLine(courtPoints[2], courtPoints[3], image, color);
    DrawDebug::drawLine(courtPoints[3], courtPoints[0], image, color);

    DrawDebug::drawLine(courtPoints[4], courtPoints[5], image, color);
    DrawDebug::drawLine(courtPoints[6], courtPoints[7], image, color);

    DrawDebug::drawLine(courtPoints[8], courtPoints[9], image, color);
}


float VolleyballCourtModel::evaluateModel(const std::vector<cv::Point2f>& courtPoints, const cv::Mat& binaryImage)
{
    float score = 0;

    // TODO: heuritic to see whether the model makes sense
    float d1 = distance(courtPoints[0], courtPoints[1]);
    float d2 = distance(courtPoints[1], courtPoints[2]);
    float d3 = distance(courtPoints[2], courtPoints[3]);
    float d4 = distance(courtPoints[3], courtPoints[0]);
    float t = 30;
    if (d1 < t || d2 < t || d3 < t || d4 < t)
    {
        return initialFitScore;
    }

    score += computeScoreForLineSegment(courtPoints[0], courtPoints[1], binaryImage);
    score += computeScoreForLineSegment(courtPoints[1], courtPoints[2], binaryImage);
    score += computeScoreForLineSegment(courtPoints[2], courtPoints[3], binaryImage);
    score += computeScoreForLineSegment(courtPoints[3], courtPoints[0], binaryImage);
    score += computeScoreForLineSegment(courtPoints[4], courtPoints[5], binaryImage);
    score += computeScoreForLineSegment(courtPoints[6], courtPoints[7], binaryImage);
    score += computeScoreForLineSegment(courtPoints[8], courtPoints[9], binaryImage);
    //score += computeScoreForLineSegment(courtPoints[10], courtPoints[11], binaryImage);
    //score += computeScoreForLineSegment(courtPoints[12], courtPoints[13], binaryImage);
//  score += computeScoreForLineSegment(courtPoints[14], courtPoints[14], binaryImage);

//  std::cout << "Score = " << score << std::endl;

    return score;
}

float VolleyballCourtModel::computeScoreForLineSegment(const cv::Point2f& start, const cv::Point2f& end,
                                                   const cv::Mat& binaryImage)
{
    float score = 0;
    float fgScore = 1;
    float bgScore = -0.5;
    int length = round(distance(start, end));

    Point2f vec = normalize(end-start);

    for (int i = 0; i < length; ++i)
    {
        Point2f p = start + i*vec;
        int x = round(p.x);
        int y = round(p.y);
        if (isInsideTheImage(x, y, binaryImage))
        {
            uchar imageValue = binaryImage.at<uchar>(y,x);
            if (imageValue == fgValue)
            {
                score += fgScore;
            }
            else
            {
                score += bgScore;
            }
        }
    }
    return score;
}

cv::Point2f VolleyballCourtModel::normalize(const cv::Point2f& v)
{
    float l = sqrt(v.x*v.x + v.y*v.y);//the length
    return cv::Point2f(v.x/l, v.y/l);
}


float VolleyballCourtModel::distance(const cv::Point2f& p1, const cv::Point2f& p2)
{
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return sqrt(dx*dx + dy*dy);
}

bool VolleyballCourtModel::isInsideTheImage(float x, float y, const cv::Mat& image)
{
    return (x >= 0 && x < image.cols) && (y >= 0 && y < image.rows);
}

void VolleyballCourtModel::writeToFile(const std::string& filename)
{
    std::vector<Point2f> transformedModelPoints(10);//changed to 10 from 16
    perspectiveTransform(courtPoints, transformedModelPoints, transformationMatrix);

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        throw std::runtime_error("Unable to open file: " + filename);
    }
    for (auto& point: transformedModelPoints)
    {
        outFile << point.x << ";" << point.y << std::endl;
    }
}
