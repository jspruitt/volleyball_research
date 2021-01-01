//
// Created by Jenny Pruitt on 5/3/19.
//

#include "Plotter.h"

#include <boost/tuple/tuple.hpp>
#include <cmath>
using namespace std;
Plotter::Plotter(){
    currentFrame = 0;
    myXFile.open ("xFile.dat");
    myYFile.open("yFile.dat");
    maxFrame = INT_MAX;
    minFrame = 0;
}
void Plotter::setMaxFrame(int mF){
    maxFrame = mF;
}
void Plotter::setMinFrame(int mF){
    minFrame = mF;
}
Plotter::Plotter(int mF){
    currentFrame = 0;
    myXFile.open ("xFile.dat");
    myYFile.open("yFile.dat");
    maxFrame = mF;
}
void Plotter::plot(){
    myXFile.close();
    myYFile.close();
    Gnuplot gp;
    gp << "f(x) = a*x**2 + b*x + c \n";
    gp << "fit f(x) 'yFile.dat' via a, b, c \n";
    gp << "plot 'yFile.dat' , f(x)\n";
}
Plotter::~Plotter() {
    myXFile.close();
    myYFile.close();

}
void Plotter::stepFrame(){
    currentFrame++;
}
void Plotter::addDataPointX(double xPoint, int color){
    //TODO: Debug
    if(currentFrame == maxFrame)
        plot();
    if(currentFrame <= maxFrame && currentFrame >= minFrame)
        myXFile << currentFrame <<"     "<< xPoint << "     " << color <<endl;
}
void Plotter::addBreak(){
    myXFile<<"     "<<endl;
    myYFile<<"     "<<endl;
}
void Plotter::addDataPointY(double yPoint, int color){
    if(currentFrame <= maxFrame && currentFrame >= minFrame)
        myYFile << currentFrame <<"     "<< yPoint << "     " << color << endl;
}
