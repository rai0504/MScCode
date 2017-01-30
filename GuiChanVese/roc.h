#ifndef ROC_H
#define ROC_H

#include <opencv2/core/core.hpp>
#include <stdexcept>
#include <iostream>

class ROC
{
public:
    ROC(cv::Mat* segmentationresult, cv::Mat* groundtruth);

    unsigned long tp();
    unsigned long hit();
    unsigned long tn();
    unsigned long correctRejection();
    unsigned long fp();
    unsigned long falseAlarm();
    unsigned long fn();
    unsigned long miss();
    unsigned long population();

    unsigned long conditionPositive();
    unsigned long conditionNegative();
    unsigned long predictedConditionPositive();
    unsigned long predictedConditionNegative();

    double falseOmissionRate();
    double positiveLikelihoodRatio();
    double lr_plus();
    double negativeLikelihoodRatio();
    double lr_minus();
    double diagnosticOddsRatio();
    double dor();
    double prevalence();

    double positivePredictiveValue();
    double ppv();
    double precision();

    double recall();
    double truePositiveRate();
    double tpr();
    double sensitivity();
    double hitRate();

    double trueNegativeRate();
    double specificity();
    double tnr();

    double negativePredictiveValue();
    double npv();

    double fallout();
    double falsePositiveRate();
    double fpr();

    double falseDiscoveryRate();
    double fdr();

    double missRate();
    double falseNegativeRate();
    double fnr();

    double fBetaScore(double beta);

    double accuracy();
    double f1Score();
    double matthewsCorrelationCoefficient();
    double mcc();

    double informedness();
    double markedness();

private:
    unsigned long true_positives;
    unsigned long true_negatives;
    unsigned long false_positives;
    unsigned long false_negatives;
};

#endif // ROC_H
