#ifndef MIX_H
#define MIX_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <sstream>
#include <math.h>
using namespace std;

double max(double a, double b, double c);
void mixMaximumProbability(cv::Mat *r, cv::Mat *g, cv::Mat *b, cv::Mat *y);
void mixWeightedMix(cv::Mat *r, cv::Mat *g, cv::Mat *b, double rw, double gw, double bw, cv::Mat *y);

#endif // MIX_H
