#ifndef SEGMENT_H
#define SEGMENT_H

#include <QThread>
#include "customdefines.h"
#include "runningstatistics.h"
#include "pushrelabel.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

class Segment : public QThread
{
    Q_OBJECT
public:
    explicit Segment(QObject *parent = 0);
    void run();
    void setNWeights(Graph *g, eInterConnectivity connectivity, double mu);
    void drawMaskOnImage(cv::Mat* img, cv::Mat* mask);
    void initialise();
    void unproposedInitialise();
    void classifyAgainst(QString gtPath);

    bool stopped=false;
    eInterConnectivity connectivity;
    double length_normalise;
    double dTheta;
    double convergence_criterion;
    int maximum_iterations;
    double c0=0, c1=0;
    bool dilate = false;
    int dilate_size;
    int dilate_element;
    double mu, l1, alpha, l0, pe, h;

    cv::Mat rawImage;
    cv::Mat binaryVariables;
    cv::Mat segmented;
    cv::Mat gtImage;

    QString algo;
    QString method;
    QString sRawImage;
    //QString sGtImage;

    Graph *g;

signals:
    void cycleComplete(QString);
    void parametersEstimated(double,double,double,double,double,double);
    void segmentationComplete();
    void classificationValue(QString);
};

#endif // SEGMENT_H
