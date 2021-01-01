//
// Created by Jenny Pruitt on 5/3/19.
//

#ifndef CPP_COURT_LINE_PLOTTER_H
#define CPP_COURT_LINE_PLOTTER_H
#include "gnuplot-iostream.h"
#include <iostream>

class Plotter {
public:
    Plotter(int mF);
    Plotter();
    ~Plotter();
    void stepFrame();
    void addDataPointX(double xPoint, int color);
    void addDataPointY(double yPoint, int color);
    void addBreak();
    void setMaxFrame(int mF);
    void setMinFrame(int mF);
    void plot();
private:
    int currentFrame;
    std::ofstream myXFile, myYFile;
    int maxFrame, minFrame;
};


#endif //CPP_COURT_LINE_PLOTTER_H
