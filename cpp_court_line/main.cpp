//
// Created by Jenny Pruitt on 6/16/19.
// this is to try out feature trackers

// g++ $(pkg-config --cflags --libs /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv4.pc) -std=c++11  court_line.cpp BallCandidateProducing.cpp DrawDebug.cpp VolleyballCourtFitter.cpp VolleyballCourtModel.cpp PixelDetection.cpp LineDetection.cpp
#include "opencv2/opencv.hpp"
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <iostream>
#include <cmath>
#include "Line.h"
#include "Plotter.h"
#include "LineComparator.cpp"
#include "VolleyballCourtModel.h"
#include "DrawDebug.h"
#include "VolleyballCourtFitter.h"
#include "PixelDetection.h"
#include "LineDetection.h"
#include "InitialBallTrack.h"

using namespace std;
using namespace cv;
#define PI 3.14159265
#define fgValue 255
#define bgValue 0


cv::Mat getFrame(VideoCapture v){
    Mat frame;
    v >> frame;
    transpose(frame, frame);
    flip(frame, frame, 0);
    return frame;
}

int main(int argc, char** argv){
    Mat frame, white, hough;
    String file;
    vector<Line> lines;
    if(argc == 2)
        file =  argv[1];
    else
        file = "sr_video_1-34.mov";
    VideoCapture vc(file);

    frame = getFrame(vc);


    PixelDetection p;
    white = p.run(frame);

    LineDetection l;
    lines = l.run(white, frame);

    VolleyballCourtFitter v;
    VolleyballCourtModel final = v.run(lines, white, frame);
    final.drawModel(frame);
    final.specialPoints(frame);
    DrawDebug::displayImage(frame);

    VideoWriter outputVideo;
    int ex = static_cast<int>(vc.get(CAP_PROP_FOURCC));
    Size S = Size((int) frame.cols, (int) frame.rows);  //Acquire input size
    outputVideo.open("output.mp4" , ex, vc.get(CAP_PROP_FPS),S, true);


    vector<float> d = final.getLineDistances();
    InitialBallTrack b(d);
    b.printOut();
    //for frame tracking
    Mat frameOne = frame;
    Mat frameTwo;
    frameTwo = getFrame(vc);
    int countFrame = 0;
    vector<KeyPoint> keypoints;

    Mat current;
    Ptr<Tracker> tracker;
    Rect2d bbox;
    bool track = false;
    int smallCountFrame = 0;
    while(countFrame < 300) {
        current = getFrame(vc);
        if (current.empty())
            break;
        if (track) {
            if (current.empty())
                break;
            bool ok = tracker->update(current, bbox);
            Mat rectFrame;
            if (ok) {
                rectFrame = DrawDebug::drawRectangle(bbox, current);
            }
            DrawDebug::displayImage(rectFrame, 1);
            smallCountFrame++;
            if (smallCountFrame > 6) { //ends if it exceeds 4 frames
                track = false;
                smallCountFrame = 0;
            }
        } else {
            keypoints = b.run(frameOne, current);
            Mat ballFrame = DrawDebug::drawBalls(keypoints, current);
            final.drawModel(ballFrame);
            DrawDebug::displayImage(ballFrame, 0);
            if (keypoints.size() == 1) { //ends if it finds a ball, goes to tracker
                track = true;
                smallCountFrame = 0;
                tracker = TrackerMIL::create();
                Rect2d bbox(keypoints[0].pt.x - keypoints[0].size/2, keypoints[0].pt.y - keypoints[0].size/2,
                            keypoints[0].size,
                            keypoints[0].size);
                tracker->init(current, bbox);
                DrawDebug::displayImage(DrawDebug::drawRectangle(bbox, current), 0);
            }
        }
        frameOne = frameTwo;
        frameTwo = current;
        countFrame++;
        outputVideo << current;
    }
//
//        while(countFrame < 300) {
//        while (keypoints.size() == 0) {
//            current = getFrame(vc);
//
//            if (current.empty())
//                break;
//            keypoints = b.run(frameOne, current);
//            Mat ballFrame = DrawDebug::drawBalls(keypoints, current);
//            final.drawModel(ballFrame);
//            DrawDebug::displayImage(ballFrame, 1);
//            frameOne = frameTwo;
//            frameTwo = current;
//            countFrame++;
//            outputVideo << ballFrame;
//        }
//        Ptr<Tracker> tracker;
//        tracker = TrackerMIL::create();
//
//        Rect2d bbox(keypoints[0].pt.x - keypoints[0].size, keypoints[0].pt.y - keypoints[0].size, keypoints[0].size * 2,
//                    keypoints[0].size * 2);
//
//        tracker->init(current, bbox);
//
//        DrawDebug::displayImage(DrawDebug::drawRectangle(bbox, current), 0);
//
//        for(int i = 0; i < 5; i++) {
//            current = getFrame(vc);
//
//            if (current.empty())
//                break;
//            bool ok = tracker->update(current, bbox);
//            Mat rectFrame;
//            if (ok) {
//                rectFrame = DrawDebug::drawRectangle(bbox, frame);
//            }
//            DrawDebug::displayImage(rectFrame, 1);
//            frameOne = frameTwo;
//            frameTwo = current;
//            countFrame++;
//            outputVideo << current;
//        }
//    }
    b.endPlot();
    return 0;
}