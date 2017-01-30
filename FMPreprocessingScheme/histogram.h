#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "bezier.h"

const int number_of_bins = 256;
const int histogram_width = 600;
const int histrogram_height = 400;
const int bin_width = (int) round((double)histogram_width/number_of_bins);

const int window_width = 512;
const int window_height = 400;

void plotHistogram(cv::Mat* histogram);
void plotLine(cv::String windowName, cv::Point startPoint, cv::Point endPoint, cv::Scalar color);

#endif // HISTOGRAM_H
