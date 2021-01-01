//
// Created by Jenny Pruitt on 4/29/19.
//

#ifndef CPP_COURT_LINE_BALLCANDIDATEPRODUCING_H
#define CPP_COURT_LINE_BALLCANDIDATEPRODUCING_H

#include <cmath>
#include "opencv2/opencv.hpp"
#include "PixelDetection.h"
#include "Plotter.h"


using namespace cv;
using namespace std;
class BallCandidateProducing {
public:
    BallCandidateProducing(std::vector<float> dist);
    void printOut();
    cv::Mat run(cv::Mat startFrame, cv::Mat endFrame);
    cv::Mat detectMovement(cv::Mat startFrame, cv::Mat endFrame);
    cv::Mat siftCandidates(cv::Mat movementFrames, cv::Mat frame);
    void findDerivatives();
    void printDerivatives();
    void endPlot();
private:
    float d, l, deltar;
    vector<float> rLimits;
    Plotter plotter;
    bool inFrame;
    vector<float> points;
    vector<float> derivatives;
    vector<float> secondDerivatives;

};


#endif //CPP_COURT_LINE_BALLCANDIDATEPRODUCING_H
