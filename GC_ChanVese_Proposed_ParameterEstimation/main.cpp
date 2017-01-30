#define _USE_MATH_DEFINES
#include <cmath>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "runningstatistics.h"
#include "graph.h"
#include "pushrelabel.h"

#undef STATIC_G

enum eInterConnectivity
{
    FOUR_WAY = 0,
    EIGHT_WAY
};

void convertTo3ChannelGrayScale(cv::Mat* img, cv::Mat *output);

void updateArithemeticMeans(cv::Mat* img, cv::Mat *randomVariable, double *c0, double *c1);
void estimateParameters(double c0, double c1, double *pe, double *h, double *mu, double *l1, double *alpha);

double eP(double xp, double u, double l0, double l1, double c0, double c1);
void setTWeights(Graph *g, cv::Mat *image, double l0, double l1, double c0, double c1);
void setNWeights(Graph *g, eInterConnectivity connectivity, double mu);
void updateBinaryVariables(Graph *g, cv::Mat *segmentationMask);
void getMorphContour(cv::Mat* binaryVariable, cv::Mat* contour);
void getCannyContour(cv::Mat* binaryVariable, cv::Mat* contour);
void drawContourOnImage(cv::Mat* img, cv::Mat* contour, cv::Mat* output);
void drawMaskOnImage(cv::Mat* img, cv::Mat* mask, cv::Mat* output);

const int ESC = 27;
const double convergence_criterion = 1e-3;
const int maximum_iterations = 50;
const double length_normalise = 1/(M_PI/8*2);
const double kp = 0.154494;
const double kh = 0.412737;
const double flowmax = 2*sqrt(2) + 4;
const double e = 1/sqrt(2.0);
const double dTheta = M_PI/8;

double c0=0, c1=0;
bool dilate = true;

#ifndef STATIC_G
Graph *g;
#endif

string algo = "emgmm2";

int main()
{
    // get image from user
    string rawImgPath;
    cout << "Path to image: ";
    cin >> rawImgPath;

    // load black and white image
    cv::Mat rawImage;
    rawImage = cv::imread(rawImgPath, CV_LOAD_IMAGE_GRAYSCALE);
    if (rawImage.empty())
    {
        cout << "Error reading image..." << endl << "exiting..." << endl;
        return -1;
    }

    cout << "Intialisation type [default,otsu, kmeans, emgmm]: ";
    cin >> algo;
    cv::Mat binaryVariables;

    if (algo.compare("otsu")==0)
    {
        // OTSU binarization
        binaryVariables = cv::Mat::zeros(rawImage.rows, rawImage.cols, CV_8UC1);
        cv::threshold(rawImage, binaryVariables, 0, 1, CV_THRESH_BINARY+CV_THRESH_OTSU);
    }
    else if (algo.compare("kmeans")==0)
    {
        // K-MEANS (k=2)
        binaryVariables = cv::Mat::zeros(rawImage.rows, rawImage.cols, CV_8UC1);
        cv::Mat inputData;
        rawImage.convertTo(inputData,CV_32FC1);
        inputData = inputData.reshape(1,inputData.rows*inputData.cols);
        cv::Mat centers;
        cv::kmeans(inputData,2,binaryVariables, cv::TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 0.1),3,cv::KMEANS_RANDOM_CENTERS,centers);
        binaryVariables = binaryVariables.reshape(1,rawImage.rows);
        binaryVariables.convertTo(binaryVariables,CV_8UC1);
    }
    else if (algo.compare("emgmm")==0)
    {
        // EM-GMM
        binaryVariables = cv::Mat::zeros(rawImage.rows, rawImage.cols, CV_8UC1);
        cv::Ptr<cv::ml::EM> emGMM = cv::ml::EM::create();
        emGMM->setClustersNumber(2);
        emGMM->setCovarianceMatrixType(cv::ml::EM::COV_MAT_DEFAULT);
        emGMM->setTermCriteria(cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 300, 0.1));
        emGMM->clear();

        cv::Mat inputDataE;
        rawImage.convertTo(inputDataE,CV_32FC1);
        inputDataE = inputDataE.reshape(1,rawImage.rows*rawImage.cols);
        emGMM->trainEM(inputDataE, cv::noArray(), binaryVariables, cv::noArray());
        binaryVariables = binaryVariables.reshape(1,rawImage.rows);
        binaryVariables.convertTo(binaryVariables,CV_8UC1);
    }
    else if (algo.compare("default")==0)
    {
        // center circle
        binaryVariables = cv::Mat::zeros(rawImage.rows, rawImage.cols, CV_8UC1);
        int radius = 0;
        if (rawImage.rows>rawImage.cols) radius = rawImage.cols/2;
        else radius = rawImage.rows/2;
        cv::circle(binaryVariables, cv::Point(rawImage.rows/2, rawImage.cols/2), radius, cv::Scalar(255), -1, 8, 0);
    }
    else
    {
        cout << "Initialisation scheme not defined...exiting";
    }

    /* Normalise for further processing */
    rawImage.convertTo(rawImage,CV_32FC1);
    rawImage = rawImage/255;

    int dilation_size = 0;
    cout << "Initial mask dilation [0-10]: ";
    cin >> dilation_size;
    if (dilation_size>0)
    {
        int dilation_type;
        int dilation_elem = 2;
        if( dilation_elem == 0 ){ dilation_type = cv::MORPH_RECT; }
        else if( dilation_elem == 1 ){ dilation_type = cv::MORPH_CROSS; }
        else if( dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }
        cv::Mat element = cv::getStructuringElement( dilation_type,
                                                     cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                                     cv::Point( dilation_size, dilation_size ) );
        cv::dilate(binaryVariables,binaryVariables,element);
        updateArithemeticMeans(&rawImage, &binaryVariables, &c0, &c1);
    }

    updateArithemeticMeans(&rawImage, &binaryVariables, &c0, &c1);
    // order means in ascending order
    if (c0>c1)
    {
        binaryVariables = cv::Mat::ones(rawImage.rows,rawImage.cols,CV_8UC1)-binaryVariables; // this is to ensure the fg is labelled as one
        double temp = c1;
        c1 = c0;
        c0 = temp;
    }
    cout << "----- OTSU: Initial: c0 = " << c0 << ", c1 = " << c1 << endl;

    string segmentationmethod="proposed";
    cout << "Choose parameter estimation method [proposed, el-zehiry, masaka]: ";
    cin >> segmentationmethod;

    double mu, l1, alpha, l0, pe=-1, h=-1;
    if (segmentationmethod.compare("proposed")==0)
    {
        estimateParameters(c0,c1,&pe,&h,&mu,&l1,&alpha);
        mu *= length_normalise;
        l0 = alpha*l1;
        cout << "----- pe = " << pe << ", h = " << h << ", alpha = " << alpha << endl;
    }
    else if (segmentationmethod.compare("el-zehiry")==0)
    {
        mu = 6502.5;
        l0 = 1;
        l1 = 1;
    }
    else if (segmentationmethod.compare("masaka")==0)
    {
        mu = 0.01;
        l0 = 85;
        l1 = 1;
    }
    else
    {
        cout << "Segmentation parameters not specified...exiting";
        return 0;
    }
    cout << "----- mu = " << mu << ", l0 = " << l0 << ", l1 = " << l1 << endl;

    string final;
    cout << "Save final segmentation as mask or contour [mask, contour]: ";
    cin >> final;
    bool saveContour = 0; // mask = 0, contour = 1
    if (final.compare("contour")==0) saveContour = 1;

    cout << endl << "----------" << endl << "Starting segmentation" << endl;
    int iteration = 0;
    double mean_difference = convergence_criterion+1;
    // while means is greater than convergence criteria or less than maximum iterations
    while (mean_difference>convergence_criterion && iteration<maximum_iterations)
    {
        // construct the graph
        g = new Graph(rawImage.rows, rawImage.cols);
        setTWeights(g, &rawImage, l0, l1, c0, c1);
        setNWeights(g, eInterConnectivity::EIGHT_WAY, mu);

        // minimise the graph and place the mask on the random binary variables image
        PushRelabel pr(g);
        pr.calculateMaxFlow();
        updateBinaryVariables(g, &binaryVariables);

        // update means (using random binary variables image)
        double old_c0=c0, old_c1=c1;
        updateArithemeticMeans(&rawImage, &binaryVariables, &c0, &c1);
        mean_difference = abs(old_c0-c0) + abs(old_c1-c1);
        iteration++;
        cout << "Iteration "<< iteration << ": c0 = " << c0 << ", c1 = " << c1 << ", |d| = " << mean_difference << endl;
    } // while
    cout << "Complete" << endl << "----------" << endl << endl;

    cv::Mat output;
    if (saveContour)
    {
        cv::Mat contour;
        getCannyContour(&binaryVariables, &contour);
        drawContourOnImage(&rawImage, &contour, &output);
    }
    else // save segmentation mask
    {
        drawMaskOnImage(&rawImage, &binaryVariables, &output);
    }
    cv::imshow("Segmentation Result", output);

    cv::waitKey(0);
    return 0;
}

void updateArithemeticMeans(cv::Mat* img, cv::Mat *randomVariable, double *c0, double *c1)
{
    RunningStatistics r0, r1;
    for(int r=0; r<randomVariable->rows; r++)
        for(int c=0; c<randomVariable->cols; c++)
        {
            if (randomVariable->at<uchar>(r,c)>0) r1.push(img->at<float>(r,c));
            else r0.push(img->at<float>(r,c));
        }
    *c0=r0.mean();
    *c1=r1.mean();
}

void estimateParameters(double c0, double c1, double *pe, double *h, double *mu, double *l1, double *alpha)
{
    *mu=1;
    *pe = c0+kp*(c1-c0);
    *h = c0+kh*(c1-c0);
    *alpha = pow((c1-c0)/((*pe)-c0)-1,2);
    *l1 = ((*mu)*flowmax)/((*alpha)*pow((*h)-c0,2)-pow((*h)-c1,2));
}

double eP(double xp, double u, double l0, double l1, double c0, double c1)
{
    if (xp==0) return l0*pow(u-c0,2);
    else return l1*pow(u-c1,2);
}

void setTWeights(Graph *g, cv::Mat *image, double  l0, double l1, double c0, double c1)
{
    // Dense Data Energy Connections
    for (int r=0; r<g->r; r++)
        for (int c=0; c<g->c; c++)
        {
            // add an edge from the source, S
            g->addEdge(g->s, g->toIndex(r, c), eP(1, image->at<float>(r,c), l0, l1, c0, c1));
            // add an edge to the sink, T
            g->addEdge(g->toIndex(r, c), g->t, eP(0, image->at<float>(r,c), l0, l1, c0, c1));
        }
}

void setNWeights(Graph *g, eInterConnectivity connectivity, double mu)
{
    /*
     *  Epq = | Epq(0,0)    Epq(0,1) | = | A    B | = A|  0      0  | + | 0    D-C | + | 0    B+C-A-D |
     *        | Epq(1,0)    Epq(1,1) |   | C    D |    | C-A    C-A |   | 0    D-C |   | 0       0    |
     *
     *  but A = D = 0 : B+C-A-D = B+C = Epq(0,1) + Epq(1,0) = Wpq + Wpq = 2.Wpq
     */
    for (int r=0; r<g->r; r++)
        for (int c=0; c<g->c; c++)
        {
            if (connectivity==eInterConnectivity::EIGHT_WAY)
            {
                     if (r>0)
                     {
                                         g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), 2*mu*dTheta);    // UP
                         if (c<g->c-1)   g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), 2*mu*dTheta*e);  // UP-RIGHT
                         if (c>0)        g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), 2*mu*dTheta*e);  // UP-LEFT
                     }
                     if (c<g->c-1)       g->addEdge(g->toIndex(r, c), g->toIndex(r, c+1), 2*mu*dTheta);    // RIGHT
                     if (r<g->r-1)
                     {
                                         g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), 2*mu*dTheta);    // BOTTOM
                         if (c<g->c-1)   g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), 2*mu*dTheta*e);  // BOTTOM-RIGHT
                         if (c>0)        g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), 2*mu*dTheta*e);  // BOTTOM-LEFT
                     }
                     if (c>0)            g->addEdge(g->toIndex(r, c), g->toIndex(r, c-1), 2*mu*dTheta);    // LEFT
            }
            else
            {
                    /*FOUR_WAY*/
                     if (r>0)           g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), 2*mu*dTheta);  // UP
                     if (c<g->c-1)      g->addEdge(g->toIndex(r, c), g->toIndex(r, c+1), 2*mu*dTheta);  // RIGHT
                     if (r<g->r-1)      g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), 2*mu*dTheta);  // BOTTOM
                     if (c>0)           g->addEdge(g->toIndex(r, c), g->toIndex(r, c-1), 2*mu*dTheta);  // LEFT
            }

        }
}

void updateBinaryVariables(Graph *g, cv::Mat *segmentationMask)
{
    for(int r=0; r<g->r; r++)
        for(int c=0; c<g->c; c++)
        {
            if (g->node[g->toIndex(r, c)].distance>=g->r*g->c+2) segmentationMask->at<uchar>(r, c) = 0;
            else segmentationMask->at<uchar>(r, c) = 1;
        }
}

void getMorphContour(cv::Mat* binaryVariable, cv::Mat* contour)
{
    int apertureSize = 3;
    cv::Mat temp;
    cv::blur(*binaryVariable, temp, cv::Size(apertureSize,apertureSize));
    int morph_elem = 2; // 0- rect, 1-cross, 2-ellipse
    int morph_size = 1; // outline width
    cv::Mat element = cv::getStructuringElement( morph_elem, cv::Size( 2*morph_size + 1, 2*morph_size+1 ), cv::Point(morph_size, morph_size));
    cv::morphologyEx(temp, *contour, CV_MOP_GRADIENT, element);
}

void getCannyContour(cv::Mat* binaryVariable, cv::Mat* contour)
{
    cv::Mat temp = *binaryVariable*255;
    cv::Canny(temp,*contour,50, 200);
}

void convertTo3ChannelGrayScale(cv::Mat* img, cv::Mat* output)
{
    cv::Mat channel[3];
    img->copyTo(channel[0]);
    img->copyTo(channel[1]);
    img->copyTo(channel[2]);
    cv::merge(channel, 3, *output);
}

void drawContourOnImage(cv::Mat* img, cv::Mat* contour, cv::Mat* output)
{
    cv::Mat temp1, splitchannel[3];
    convertTo3ChannelGrayScale(img, &temp1);
    cv::split(temp1, splitchannel);

    cv::Mat inverseContour = cv::Mat::ones(img->rows, img->cols, CV_32FC1);
    cv::Mat tempContour;
    contour->convertTo(tempContour,CV_32FC1);
    inverseContour = inverseContour - tempContour;
    splitchannel[0] = splitchannel[0].mul(inverseContour);
    splitchannel[0].copyTo(splitchannel[1]);
    splitchannel[0].copyTo(splitchannel[2]);

    splitchannel[2] = splitchannel[2]+tempContour;

    cv::merge(splitchannel, 3, *output);
}

void drawMaskOnImage(cv::Mat* img, cv::Mat* mask, cv::Mat* output)
{
    cv::Mat temp1, splitchannel[3];
    convertTo3ChannelGrayScale(img, &temp1);
    cv::split(temp1, splitchannel);

    cv::Mat tempMask;
    mask->convertTo(tempMask,CV_32FC1);
    splitchannel[2] = splitchannel[2]+tempMask;

    cv::merge(splitchannel, 3, *output);
}
