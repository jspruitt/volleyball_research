//
// Created by Jenny Pruitt on 4/14/19.
//

#include "LineDetection.h"
#include "LineComparator.cpp"

using namespace std;
using namespace cv;
#define PI 3.14159265
#define fgValue 255
#define bgValue 0

int const refinementIterations = 20; //refine
int const minHoughVotesThreshold =  120;
int const distanceThreshold = 8;


vector<Line> LineDetection::houghTransform(Mat white){
    vector<Vec2f> lines;
    vector<Line> officiaLines;
    HoughLines( white, lines, 1, CV_PI/180, minHoughVotesThreshold, 0, 0 );

    for( size_t i = 0; i < lines.size(); i++ )
    {
        officiaLines.push_back(Line::fromRhoTheta(lines[i][0], lines[i][1]));
    }
    return officiaLines;
}


Mat LineDetection::getClosePointsMatrix(Line line, const Mat& binaryImage, const Mat& rgbImage)
{
    Mat M = Mat::zeros(0, 2, CV_32F);
    for (unsigned int x = 0; x < binaryImage.cols; ++x) {
        for (unsigned int y = 0; y < binaryImage.rows; ++y) {
            if (binaryImage.at<uchar>(y, x) == fgValue) {
                float distance = line.getDistance(Point2f(x, y));
                if (distance < distanceThreshold) {
                    Mat point = Mat::zeros(1, 2, CV_32F);
                    point.at<float>(0, 0) = x;
                    point.at<float>(0, 1) = y;
                    M.push_back(point);
                }
            }
        }
    }
    return M;
}

Line LineDetection::getRefinedParameters(Line line, Mat binaryImage, Mat rgbImage)
{
    Mat A = getClosePointsMatrix(line, binaryImage, rgbImage);
    Mat X = Mat::zeros(1, 4, CV_32F);
    fitLine(A, X, DIST_L2, 0, 0.01, 0.01);
    Point2f v(X.at<float>(0,0), X.at<float>(0,1));
    Point2f p(X.at<float>(0,2), X.at<float>(0,3));
    return Line(p, v);
}

void LineDetection::refineLineParameters(vector<Line>& lines, const Mat& binaryImage, const Mat& rgbImage)
{
    for (auto& line: lines)
    {
        line = getRefinedParameters(line, binaryImage, rgbImage);
    }

}

bool lineEqual(const Line& a, const Line& b)
{
    return a.isDuplicate(b);
}

void LineDetection::removeDuplicateLines(std::vector<Line>& lines, const cv::Mat& rgbImage)
{
    auto it = std::unique(lines.begin(), lines.end(), lineEqual);
    lines.erase(it, lines.end());
}
std::vector<Line> LineDetection::run(const cv::Mat& binaryImage, const cv::Mat& rgbImage){
    vector<Line> lines;
    lines = houghTransform(binaryImage);
    for(int i = 0; i < refinementIterations; i++){
        refineLineParameters(lines, binaryImage, rgbImage);

        removeDuplicateLines(lines, rgbImage);
    }
    return lines;
}