#include "mix.h"

double max(double a, double b, double c)
{
    double maxVal = a;
    if (b>maxVal) maxVal=b;
    if (c>maxVal) maxVal=c;

    return maxVal;
}

void mixMaximumProbability(cv::Mat *r, cv::Mat *g, cv::Mat *b, cv::Mat *y)
{
    *y = cv::Mat::zeros(r->rows, r->cols, CV_8UC1);
    for(int row=0; row<r->rows; row++)
        for(int col=0; col<r->cols; col++)
        {
            double pixel = max((double)r->at<uchar>(row,col), (double)g->at<uchar>(row,col), (double)b->at<uchar>(row,col));
            y->at<uchar>(row,col) = (uchar) round(pixel);
        }
}

void mixWeightedMix(cv::Mat *r, cv::Mat *g, cv::Mat *b, double rw, double gw, double bw, cv::Mat *y)
{
    *y = cv::Mat::zeros(r->rows, r->cols, CV_8UC1);
    for(int row=0; row<r->rows; row++)
        for(int col=0; col<r->cols; col++)
        {
            double pixel = rw*(r->at<uchar>(row,col)) + gw*(g->at<uchar>(row,col)) + bw*(b->at<uchar>(row,col));
            y->at<uchar>(row,col) = (uchar) round(pixel/(rw+gw+bw));
        }
}
