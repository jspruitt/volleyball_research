//
// Created by Jenny Pruitt on 4/29/19.
//

#include "BallCandidateProducing.h"
#include "DrawDebug.h"

#define PI 3.14159

BallCandidateProducing::BallCandidateProducing(std::vector<float> dist) {
    inFrame = true;
    d = 21.0;
    l = 900.0;
    deltar = 100;
    rLimits.push_back(PI * pow(d/2 * dist[0]/l, 2) + deltar);//rmin, equation in paper
    rLimits.push_back(PI * pow(d/2 * dist[1]/l, 2) - deltar); //rmax equation in paper
}

void BallCandidateProducing::endPlot(){
    plotter.plot();
}
void BallCandidateProducing::printOut() {
    cout<< rLimits[0] << " " << rLimits[1] << endl;
}
cv::Mat BallCandidateProducing::detectMovement(cv::Mat startFrame, cv::Mat endFrame){
    Mat frameDelta, thresh, firstFrame;
    frameDelta = startFrame - endFrame;
    GaussianBlur(startFrame, startFrame, Size(9,9), 0);
    GaussianBlur(endFrame, endFrame, Size(9,9), 0);
    threshold(frameDelta, thresh, 15, 255, THRESH_BINARY);

    return thresh;
}

void BallCandidateProducing::findDerivatives(){
    int first;
    for(first = 0; first<points.size(); first++){
        if(points[first]!=0)
            break;
    }


}
void printDerivatives(){

}

cv::Mat BallCandidateProducing::siftCandidates(cv::Mat movementFrames, cv::Mat frame){

    vector<KeyPoint> keypoints;
    bitwise_not(movementFrames, movementFrames);
    Mat output = frame.clone();

    SimpleBlobDetector::Params params;

// Change thresholds
    params.minThreshold = 0;
    params.maxThreshold = 255;


// Filter by Area.
    params.filterByArea = true;
    //params.minArea = rLimits[0];
    params.maxArea = rLimits[1];



    //to find which ones are contacted
    //params.minDistBetweenBlobs = (rLimits[0] + rLimits[1])/2;

    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    detector->detect( movementFrames, keypoints);


    if(keypoints.size()==0)
    {
        if(inFrame)
            plotter.addBreak();
        else
            //circle(output, Point2f(40,40), 40, Scalar(255, 0, 0), FILLED);
        points.push_back(0);
    }
    bitwise_not(movementFrames, movementFrames);
    for (size_t i = 0; i < keypoints.size(); ++i) {
        circle(output, keypoints[i].pt, 8, Scalar(255, 0, 255), FILLED);

        cv::Point2f temp(keypoints[i].pt.x, keypoints[i].pt.y + 190);

        plotter.addDataPointX(keypoints[i].pt.x, 0);
        plotter.addDataPointY(movementFrames.rows - keypoints[i].pt.y, 0);
        points.push_back(movementFrames.rows - keypoints[i].pt.y);
        if(keypoints[i].pt.y < 30)
            inFrame = false;
        else
            inFrame = true;
    }
    return output;

}



cv::Mat BallCandidateProducing::run(cv::Mat startFrame, cv::Mat endFrame) {
    PixelDetection p;
    Mat startWhiteFrame, endWhiteFrame, movementDiff, ballCandidates;
    cvtColor(startFrame, startWhiteFrame, COLOR_RGB2GRAY);
    cvtColor(endFrame, endWhiteFrame, COLOR_RGB2GRAY);
    movementDiff = detectMovement(startWhiteFrame, endWhiteFrame);
    ballCandidates = siftCandidates(movementDiff, endFrame);
    plotter.stepFrame();
    return ballCandidates;
}