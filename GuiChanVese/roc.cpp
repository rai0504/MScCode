#include "roc.h"
#include <QDebug>

// Catch divide by zero.
inline double divide (double numerator, double denominator) {
    if (denominator == 0)
        throw std::overflow_error("Divide by zero exception. ");
    return numerator/denominator;
}

ROC::ROC(cv::Mat *segmentationresult, cv::Mat *groundtruth)
{
    true_positives = 0;
    true_negatives = 0;
    false_positives = 0;
    false_negatives = 0;
    *groundtruth /= 255;
    if ((segmentationresult->rows==groundtruth->rows) &&
        (segmentationresult->cols==groundtruth->cols) &&
        (segmentationresult->channels()==groundtruth->channels()))
    {
        for(int r=0;r<segmentationresult->rows; r++)
            for(int c=0; c<segmentationresult->cols; c++)
            {
                if (segmentationresult->at<uchar>(r,c)==groundtruth->at<uchar>(r,c))
                {
                    if (segmentationresult->at<uchar>(r,c)>0) (true_positives)++;
                    else (true_negatives)++;
                }
                else
                {
                    if (segmentationresult->at<uchar>(r,c)>0) (false_positives)++;
                    else (false_negatives)++;
                }
            }
    }
}

unsigned long ROC::tp(){ return true_positives; }
unsigned long ROC::hit(){ return tp(); }
unsigned long ROC::tn(){ return true_negatives; }
unsigned long ROC::correctRejection() { return tn(); }
unsigned long ROC::fp(){ return false_positives; }
unsigned long ROC::falseAlarm(){ return fp(); }
unsigned long ROC::fn(){ return false_negatives; }
unsigned long ROC::miss(){ return fn(); }
unsigned long ROC::population(){ return true_positives+true_negatives+false_positives+false_negatives; }

unsigned long ROC::conditionPositive(){ return true_positives+false_negatives; }
unsigned long ROC::conditionNegative(){ return true_negatives+false_positives; }
unsigned long ROC::predictedConditionPositive(){ return true_positives+false_positives; }
unsigned long ROC::predictedConditionNegative(){ return true_negatives+false_negatives; }

double ROC::falseOmissionRate()
{
    double ans = -1;
    try
    {
        ans = divide(false_negatives,false_negatives+true_negatives);
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}

double ROC::positiveLikelihoodRatio()
{
    double ans = -1;
    try
    {
        ans = divide(tpr(),false_positives/false_positives+true_negatives);
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::lr_plus(){ return positiveLikelihoodRatio(); }

double ROC::negativeLikelihoodRatio()
{
    double ans = -1;
    try
    {
        ans = divide(fnr(),true_negatives/false_positives+true_negatives);
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::lr_minus(){ return negativeLikelihoodRatio(); }

double ROC::diagnosticOddsRatio()
{
    double ans = -1;
    try
    {
        ans = divide(lr_plus(),(fn()*(tn()+fn()))/(tn()*(fn()+tp())));
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::dor(){ return diagnosticOddsRatio(); }

double ROC::prevalence(){
    double ans = -1;
    try
    {
        ans = divide(conditionPositive(),population());
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}

double ROC::positivePredictiveValue()
{
    double ans = -1;
    try
    {
        ans = divide(tp(),tp()+fp());
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::ppv(){ return positivePredictiveValue(); }
double ROC::precision(){ return positivePredictiveValue(); }

double ROC::recall()
{
    double ans = -1;
    try
    {
        ans = divide(true_positives,true_positives+false_negatives);
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::truePositiveRate(){ return recall(); }
double ROC::tpr(){ return recall(); }
double ROC::sensitivity(){ return recall(); }
double ROC::hitRate(){ return recall(); }

double ROC::trueNegativeRate()
{
    double ans = -1;
    try
    {
        ans = divide(true_negatives,true_negatives+false_positives);
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::specificity(){ return trueNegativeRate(); }
double ROC::tnr(){ return trueNegativeRate(); }

double ROC::negativePredictiveValue()
{
    double ans = -1;
    try
    {
        ans = divide(true_negatives,true_negatives+false_negatives);
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::npv(){ return negativePredictiveValue(); }

double ROC::fallout()
{
    double ans = -1;
    try
    {
        ans = divide(false_positives,true_negatives+false_positives);
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::falsePositiveRate(){ return fallout(); }
double ROC::fpr(){ return fallout(); }

double ROC::falseDiscoveryRate()
{
    double ans = -1;
    try
    {
        ans = divide(false_positives,true_positives+false_positives);
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::fdr(){ return falseDiscoveryRate(); }

double ROC::missRate()
{
    double ans = -1;
    try
    {
        ans = divide(false_negatives,true_positives+false_negatives);
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::falseNegativeRate(){ return missRate(); }
double ROC::fnr(){ return missRate(); }

double ROC::fBetaScore(double beta)
{
    double ans = -1;
    try
    {
        ans = divide((1+beta*beta)*(tp()*tp())/((tp()+fp())*(tp()+fn())),beta*beta*(tp()/(double)(tp()*fp()))+(tp()/(tp()+fn())));
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}

double ROC::accuracy()
{
    double ans = -1;
    try
    {
        ans = divide(tp()+tn(),tp()+tn()+fp()+fn());
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}

double ROC::f1Score(){ return fBetaScore(1); }

double ROC::matthewsCorrelationCoefficient()
{
    double ans = -1;
    try
    {
        ans = divide((double)tp()*tn()-fp()*fn(),sqrt((double)(tp()+fp())*(double)(tp()+fn())*(double)(tn()+fp())*(double)(tn()+fn())));
    }
    catch (std::overflow_error e) { std::cout << e.what(); }
    return ans;
}
double ROC::mcc(){ return matthewsCorrelationCoefficient(); }

double ROC::informedness(){ return sensitivity()+specificity()-1; }
double ROC::markedness(){ return precision()+npv()-1; }
