#ifndef BEZIER_H
#define BEZIER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <sstream>
#include <math.h>
using namespace std;

struct Point
{
    double x;
    double y;

    Point(double _x, double _y) : x(_x), y(_y){}
};

double quadraticSolve(double a, double b, double c, double *x1, double *x2);
void bezierCurve(Point p0, Point p1, Point p2, cv::Mat *curve);
void concatenateBezierCurve(Point p0, Point p2, Point p4, cv::Mat *attCurve, cv::Mat *ampCurve, cv::Mat *finalCurve);

#endif // BEZIER_H
