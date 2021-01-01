//
// Created by Jenny Pruitt on 4/9/19.
//

#include "VolleyballCourtFitter.h"
#include "LineComparator.cpp"
#include "DrawDebug.h"

#define initialFitScore -1e9

VolleyballCourtFitter::VolleyballCourtFitter() {

}
void VolleyballCourtFitter::getHorizontalAndVerticalLines(const std::vector<Line>& lines, std::vector<Line>& hLines, std::vector<Line>& vLines, const cv::Mat& rgbImage) {
    for (auto &line: lines) {
        if (line.isVertical()) {
            vLines.push_back(line);
        } else {
            hLines.push_back(line);
        }
    }
}

void VolleyballCourtFitter::sortLinesByDistanceToPoint(std::vector<Line>& lines, const cv::Point2f& point)
{
    sort(lines.begin(), lines.end(), LineComparator(point));
}

void VolleyballCourtFitter::sortVerticalLines(std::vector<Line>& vLines, const cv::Mat& rgbImage) {
    float y = rgbImage.rows / 2.0;
    sortLinesByDistanceToPoint(vLines, Point2f(0, y));
}
void VolleyballCourtFitter::sortHorizontalLines(std::vector<Line>& hLines, const cv::Mat& rgbImage) {
    float x = rgbImage.cols / 2.0;
    sortLinesByDistanceToPoint(hLines, Point2f(x, 0));
}
void VolleyballCourtFitter::findBestModelFit(const cv::Mat& binaryImage, const cv::Mat& rgbImage)
{
    float bestScore = initialFitScore;
    for (auto& hLinePair: hLinePairs)
    {
        for (auto& vLinePair: vLinePairs)
        {
            VolleyballCourtModel model;
            float score = model.fit(hLinePair, vLinePair, binaryImage, rgbImage);
            if (score > bestScore)
            {
                bestScore = score;
                bestModel = model;
            }
        }
    }

}

VolleyballCourtModel VolleyballCourtFitter::run(const std::vector<Line>& lines, const cv::Mat& binaryImage, const cv::Mat& rgbImage){
    //model fit
    vector<Line> hLines, vLines;
    getHorizontalAndVerticalLines(lines, hLines, vLines, rgbImage);
    sortHorizontalLines(hLines, rgbImage);
    sortVerticalLines(vLines, rgbImage);

    hLinePairs = VolleyballCourtModel::getPossibleLinePairs(hLines);
    vLinePairs = VolleyballCourtModel::getPossibleLinePairs(vLines);

    //std::cout << "Horizontal line pairs = " << hLinePairs.size() << std::endl;
    //std::cout << "Vertical line pairs = " << vLinePairs.size() << std::endl;

    if (hLinePairs.size() < 2 || vLinePairs.size() < 2)
    {
        throw std::runtime_error("Not enough line candidates were found.");
    }

    findBestModelFit(binaryImage, rgbImage);

    return bestModel;
}