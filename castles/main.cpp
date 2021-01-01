//
// Created by Jenny Pruitt on 6/26/19.
//

// g++ $(pkg-config --cflags --libs /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv4.pc) -std=c++11  court_line.cpp BallCandidateProducing.cpp DrawDebug.cpp VolleyballCourtFitter.cpp VolleyballCourtModel.cpp PixelDetection.cpp LineDetection.cpp
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
#include "Line.h"
#include "DominantColor.h"
#include "Plotter.h"
#include "LineComparator.cpp"
#include "VolleyballCourtModel.h"
#include "DrawDebug.h"
#include "VolleyballCourtFitter.h"
#include "PixelDetection.h"
#include "LineDetection.h"
#include "BallCandidateProducing.h"

using namespace std;
using namespace cv;
#define PI 3.14159265
#define fgValue 255
#define bgValue 0

cv::Mat getFrame(VideoCapture v){
    Mat frame;
    v >> frame;
    //transpose(frame, frame);
    //flip(frame, frame, 0);
    return frame;
}
int main(int argc, char** argv){
    Mat frame, white;
    String file;
    vector<Line> lines;
    if(argc == 2)
        file =  argv[1];
    else
        file = "IMG_1200.MOV";
    VideoCapture vc(file);
    frame = getFrame(vc);
    DominantColor d;
    d.find_dominant_colors(frame,3);


    return 0;
}