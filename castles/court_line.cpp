// g++ $(pkg-config --cflags --libs /usr/local/Cellar/opencv/4.0.1/lib/pkgconfig/opencv4.pc) -std=c++11  court_line.cpp BallCandidateProducing.cpp DrawDebug.cpp VolleyballCourtFitter.cpp VolleyballCourtModel.cpp PixelDetection.cpp LineDetection.cpp
#include "opencv2/opencv.hpp"
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
#include "BallCandidateProducing.h"

using namespace std;
using namespace cv;
#define PI 3.14159265
#define fgValue 255
#define bgValue 0
static void onMouseDown( int event,int x,int y,int,void*)
{
    cout<<x<<" "<<y<<endl;
}

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
        file = "sr_video_1-34.mov";
    VideoCapture vc(file);
    frame = getFrame(vc);

//    PixelDetection p;
//    white = p.run(frame);
//    imshow("Image",white);
//    setMouseCallback("Image",onMouseDown);
//    waitKey();
//
//    LineDetection l;
//    lines = l.run(white, frame);
//    cout<<lines.size()<<endl;
//    DrawDebug::displayImage(DrawDebug::drawLines(lines, frame));
//
//    VolleyballCourtFitter v;
//    VolleyballCourtModel final = v.run(lines, white, frame);
//    final.drawModel(frame);
//    DrawDebug::displayImage(frame);

//    VideoWriter outputVideo;
//    int ex = static_cast<int>(vc.get(CAP_PROP_FOURCC));
//    Size S = Size((int) frame.cols, (int) frame.rows);  //Acquire input size
//    outputVideo.open("output.mp4" , ex, vc.get(CAP_PROP_FPS),S, true);


    vector<float> d;
    d.push_back(750);
    d.push_back(2000);

    BallCandidateProducing b(d);
    b.printOut();
    //for frame tracking
    Mat frameOne = frame;
    Mat frameTwo;
    frameTwo = getFrame(vc);
    int countFrame = 0;
    while(true) {
        Mat frame;
        frame = getFrame(vc);

        if (frame.empty())
            break;
        Mat motionFrame = b.run(frameOne, frame);

        DrawDebug::displayImage(motionFrame, 1);
        frameOne = frameTwo;
        frameTwo = frame;
        countFrame++;
    }
    b.endPlot();
    b.findDerivatives();
    return 0;
}