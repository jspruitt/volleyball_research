//
// Created by Jenny Pruitt on 6/16/19.
//

#ifndef CASTLES_INITIALBALLTRACK_H
#define CASTLES_INITIALBALLTRACK_H

#include <cmath>
#include "opencv2/opencv.hpp"
#include "PixelDetection.h"
#include "Plotter.h"


using namespace cv;
using namespace std;
class InitialBallTrack {
public:
    InitialBallTrack(std::vector<float> dist);
    void printOut();
    std::vector<cv::KeyPoint> run(cv::Mat startFrame, cv::Mat endFrame);
    cv::Mat detectMovement(cv::Mat startFrame, cv::Mat endFrame);
    std::vector<cv::KeyPoint> siftCandidates(cv::Mat movementFrames, cv::Mat frame);
    void endPlot();
private:
    float d, l, deltar;
    vector<float> rLimits;
    Plotter plotter;
};



#endif //CASTLES_INITIALBALLTRACK_H
