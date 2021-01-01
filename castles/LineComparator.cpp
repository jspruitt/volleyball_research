#include "Line.h"
#include "opencv2/opencv.hpp"
using namespace cv;

class LineComparator
{
public:
    LineComparator(cv::Point2f point) : p(point) { }

    bool operator()(const Line& lineA, const Line& lineB)
    {
        return lineA.getDistance(p) < lineB.getDistance(p);
    }

private:
    cv::Point2f p;
};