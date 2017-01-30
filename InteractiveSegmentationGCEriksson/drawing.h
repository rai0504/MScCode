#ifndef DRAWING_H
#define DRAWING_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

void plotDistributions(cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm);
void plotHistograms(cv::Mat *fgTrainingData, cv::Mat *bgTrainingData, cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm);

#endif // DRAWING_H
