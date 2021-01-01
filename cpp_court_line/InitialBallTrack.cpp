//
// Created by Jenny Pruitt on 6/16/19.
//

#include "InitialBallTrack.h"
#include "DrawDebug.h"

#define PI 3.14159

InitialBallTrack::InitialBallTrack(std::vector<float> dist) {
    d = 21.0;
    l = 900.0;
    deltar = 100;
    rLimits.push_back(PI * pow(d/2 * dist[0]/l, 2) + deltar);//rmin, equation in paper
    rLimits.push_back(PI * pow(d/2 * dist[1]/l, 2) - deltar); //rmax equation in paper
    plotter.setMinFrame(70);
}

void InitialBallTrack::endPlot(){
    plotter.plot();
}
void InitialBallTrack::printOut() {
    cout<< rLimits[0] << " " << rLimits[1] << endl;
}
cv::Mat InitialBallTrack::detectMovement(cv::Mat startFrame, cv::Mat endFrame){
    Mat frameDelta, thresh, firstFrame;
    frameDelta = startFrame - endFrame;
    GaussianBlur(startFrame, startFrame, Size(3,3), 0);
    GaussianBlur(endFrame, endFrame, Size(3,3), 0);
    threshold(frameDelta, thresh, 15, 255, THRESH_BINARY);

    return thresh;
}

std::vector<cv::KeyPoint> InitialBallTrack::siftCandidates(cv::Mat movementFrames, cv::Mat frame){

    vector<KeyPoint> keypoints;
    bitwise_not(movementFrames, movementFrames);

    SimpleBlobDetector::Params params;

// Change thresholds
    params.minThreshold = 0;
    params.maxThreshold = 255;


// Filter by Area.
    params.filterByArea = true;
    params.minArea = rLimits[1];
    params.maxArea = rLimits[0];
// Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0.5f;
    params.maxCircularity = 1.0f;
    params.minDistBetweenBlobs = 1;


    //to find which ones are contacted
    //params.minDistBetweenBlobs = (rLimits[0] + rLimits[1])/2;

    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    detector->detect( movementFrames, keypoints);
    bitwise_not(movementFrames, movementFrames);
    for (size_t i = 0; i < keypoints.size(); ++i) {
        plotter.addDataPointX(keypoints[i].pt.x, 0);
        plotter.addDataPointY(movementFrames.rows - keypoints[i].pt.y, 0);
    }
    return keypoints;

}



std::vector<cv::KeyPoint> InitialBallTrack::run(cv::Mat startFrame, cv::Mat endFrame) {
    PixelDetection p;
    vector<KeyPoint> kp;
    Mat startWhiteFrame, endWhiteFrame, movementDiff, ballCandidates;
    startWhiteFrame = p.luminanceConvert(startFrame);
    endWhiteFrame = p.luminanceConvert(endFrame);
    movementDiff = detectMovement(startWhiteFrame, endWhiteFrame);
    kp = siftCandidates(movementDiff, endFrame);
    plotter.stepFrame();
    return kp;
}