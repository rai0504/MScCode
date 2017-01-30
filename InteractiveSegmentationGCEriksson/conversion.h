#ifndef CONVERSION_H
#define CONVERSION_H

#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>

#include "graph.h"
#include "runningstatistics.h"
#include "defines.h"

Graph *imageToGraph(cv::Mat *image, cv::Mat *seed, cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm, eInterConnectivity connectivity, double lambda, double sigmaW, double sigmaR);
void setTWeights(Graph *g, cv::Mat *image, cv::Mat *seed, cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm);
double gaussianDifference(int intensity1, int intensity2, double variance);
void calculatesImageStatistics(cv::Mat *image, eInterConnectivity connectivity, RunningStatistics *imageStats);
void setNWeights(Graph *g, cv::Mat *image, eInterConnectivity connectivity, double lambda, double sigmaW, double sigmaR);
void graphToImage(Graph *g, cv::Mat *segmentationMask);

#endif // CONVERSION_H
