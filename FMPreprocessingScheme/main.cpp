#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

#include <iostream>
#include <sstream>
#include <math.h>
using namespace std;

#include "bezier.h"
#include "mix.h"
#include "ced.h"

const double scale = 2;
cv::Mat bezierImage = cv::Mat::zeros((uchar)round(256*scale), (uchar)round(256*scale), CV_8UC3);
cv::Mat histogramImage = cv::Mat::zeros((uchar)round(256*scale), (uchar)round(256*scale), CV_8UC3);
cv::Mat cumulativeImage = cv::Mat::zeros((uchar)round(256*scale), (uchar)round(256*scale), CV_8UC3);

void plotBezierCurve(cv::Mat *curve, double scale, cv::Scalar lColor);
void plotHistogram(cv::Mat *curve, double scale, cv::Scalar lColor, double margin);
void bezierTheImage(cv::Mat* u, cv::Mat* y, cv::Mat *curve);
void getHistogram(cv::Mat* u, cv::Mat* h);
void getMinMax(cv::Mat* hist, int *min, int*max);

int main()
{
    string fPath;
    cout << "Path to image: ";
    cin >> fPath;
    cv::Mat img = cv::imread(fPath,CV_LOAD_IMAGE_COLOR);
    if (img.empty())
    {
        cout << "error loading image...exiting" << endl;
        return 1;
    }

    cv::Mat imgCh[3];
    cv::split(img,imgCh);

    cv::Mat attenuationCurve, amplificationCurve;
    cv::Mat finalCurve;

    double kappa1 = 0.17, kappa2 = 0.65;
    double R1 = 12, R2 = 225;
    double N = 12;

    cout << endl << "Remapping" << endl;
    cout << "Set k1 = ";
    cin >> kappa1;
    cout << "Set R1 = ";
    cin >> R1;
    cout << "Set k2 = ";
    cin >> kappa2;
    cout << "Set R2 = ";
    cin >> R2;
    cout << "Set N = ";
    cin >> N;

    double p2x = N, p2y = N;
    Point bp0(0,0),  bp2(p2x,p2y),  bp4(255,255);
    Point bp1(p2x-R1*cos(M_PI/4*(1+kappa1)),p2y-R1*sin(M_PI/4*(1+kappa1)));
    Point bp3(p2x+R2*cos(M_PI/4*(1+kappa2)),p2y+R2*sin(M_PI/4*(1+kappa2)));

    bezierCurve(bp0, bp1, bp2, &attenuationCurve);
    bezierCurve(bp2, bp3, bp4, &amplificationCurve);
    concatenateBezierCurve(bp0, bp2, bp4, &attenuationCurve, &amplificationCurve, &finalCurve);

    cv::Mat R, G, B;
    bezierTheImage(&imgCh[2], &R, &finalCurve);
    bezierTheImage(&imgCh[1], &G, &finalCurve);
    bezierTheImage(&imgCh[0], &B, &finalCurve);

    cv::Mat finalImage = cv::Mat(img.rows, img.cols, CV_8UC1);
    for(int r=0; r<finalImage.rows; r++)
        for(int c=0; c<finalImage.cols; c++)
        {
            uchar maxIntensity = R.at<uchar>(r,c);
            if (G.at<uchar>(r,c)>maxIntensity) maxIntensity = G.at<uchar>(r,c);
            if (B.at<uchar>(r,c)>maxIntensity) maxIntensity = B.at<uchar>(r,c);
            finalImage.at<uchar>(r,c) = maxIntensity;
        }
   // cv::imshow("Combined", finalImage);

    cout << endl << "Coherence Enhancing Diffusion with Optimised Rotation Invariance" << endl;
    double sigma;
    cout << "Set sigma = ";
    cin >> sigma;
    double rho;
    cout << "Set rho = ";
    cin >> rho;
    double tau;
    cout << "Set tau = ";
    cin >> tau;
    double n;
    cout << "Set n = ";
    cin >> n;

    cv::Mat *y = CoherenceEnhancedDiffusion(&finalImage,    // input image
                                             SCHARR,        // derivative method
                                             sigma,         // sigma
                                             rho,           // rho
                                             1e-3,          // c1
                                             1,             // c2
                                             tau,           // tau
                                             n,             // n
                                             1,             // diffusion frequency
                                             EULER          // update method
                                             );

    y->convertTo(*y, CV_8UC1);
    cv::imshow("Coherence Enhanced Diffusion Output", *y);

    cv::waitKey();

    return 0;
}

void plotBezierCurve(cv::Mat *curve, double scale, cv::Scalar lColor)
{
    // relaxed line
    cv::Scalar nLineColor = cv::Scalar(100, 100, 100);
    cv::line(bezierImage,
             cv::Point(0, round(scale*255)),
             cv::Point(round(scale*255), 0),
             nLineColor);

    for (int t=0; t<curve->cols-1; t++)
    {
        cv::line(bezierImage,
                 cv::Point(round(curve->at<double>(0,t)*scale),   round(255*scale-(curve->at<double>(1,t)*scale))),
                 cv::Point(round(curve->at<double>(0,t+1)*scale), round(255*scale-(curve->at<double>(1,t+1)*scale))),
                 lColor);

#if 0
#if 0
        cout << "P0 (" << round(curve->at<double>(0,t)*scale) << ", " << round(255*scale-(curve->at<double>(1,t)*scale)) << ")";
        cout << " -- P1 (" << round(curve->at<double>(0,t+1)*scale) << ", " << round(255*scale-(curve->at<double>(1,t+1)*scale)) << ")";
        cout << endl;
#else
        if (t%2==0) cout << endl;
        cout << "P0 (" << round(curve->at<double>(0,t)) << ", " << round((curve->at<double>(1,t))) << ")";
        cout << " -- P1 (" << round(curve->at<double>(0,t+1)) << ", " << round((curve->at<double>(1,t+1))) << ")";
        cout << "\t";
#endif
#endif
    }

    cv::imshow("Bezier Image", bezierImage);
}

void plotHistogram(cv::Mat *curve, double scale, cv::Scalar lColor, double margin)
{
    double sum=0;
    cv::Mat cumulative = cv::Mat::zeros(1,256,CV_64FC1);
    for (int t=0; t<curve->cols-1; t++)
    {
        /* vertrical strips */
        cv::line(histogramImage,
                 cv::Point(t*scale, round(255*scale)),
                 cv::Point(t*scale, round(255*scale-(curve->at<double>(0,t)*scale))),
                 lColor);

        /* continuous curve */
        cv::line(histogramImage,
                 cv::Point(t*scale, round(255*scale-(curve->at<double>(0,t)*scale))),
                 cv::Point((t+1)*scale, round(255*scale-(curve->at<double>(0,t+1)*scale))),
                 lColor);

        sum += curve->at<double>(0,t);
        cumulative.at<double>(0,t) = sum;
    }
    cumulative /= sum;
    cout << cumulative << endl << endl;

#if 0
    for (int t=0; t<curve->cols-1; t++)
    {
        /* vertrical strips */
        cv::line(cumulativeImage,
                 cv::Point(t*scale, round(255*scale)),
                 cv::Point(t*scale, round(255*scale-(255*scale*cumulative.at<double>(0,t)*scale))),
                 lColor);

        /* continous curve */
        cv::line(cumulativeImage,
                 cv::Point(t*scale, round(255*scale-(255*scale*cumulative.at<double>(0,t)*scale))),
                 cv::Point((t+1)*scale, round(255*scale-(255*scale*cumulative.at<double>(0,t+1)*scale))),
                 lColor);

        sum += curve->at<double>(0,t);
        cumulative.at<double>(0,t) = sum;
    }
#endif
    double threshold=0;
    for (int i=1; i<256; i++)
    {
        if (cumulative.at<double>(0,i)>margin/100)
        {
            // compare the previous one and this one and see which is closer
            double previous = cumulative.at<double>(0,i-1);
            double now = cumulative.at<double>(0,i);
            if (abs(previous-margin/100) < abs(now-margin/100))
            {
                threshold = i-1;
                cout << "Threshold = " << threshold << endl;
                break;
            }
            else
            {
                threshold = i;
                cout << "Threshold = " << threshold << endl;
                break;
            }
        }
    }
    cv::line(histogramImage,
             cv::Point(threshold, round(255*scale)),
             cv::Point(threshold, 0),
             cv::Scalar(255,255,255));
    cv::imshow("Histogram Image", histogramImage);

    //cv::imshow("Cumulative Image", cumulativeImage);
}

void bezierTheImage(cv::Mat* u, cv::Mat* y, cv::Mat *curve)
{
    u->copyTo(*y);
    for(int r=0; r<y->rows; r++)
        for(int c=0; c<y->cols; c++)
        {
            y->at<uchar>(r,c) = (uchar) round(curve->at<double>(1,y->at<uchar>(r,c)));
        }
}

void getHistogram(cv::Mat* u, cv::Mat* h)
{
    *h = cv::Mat::zeros(1,256,CV_64FC1);

    for(int i=0; i<u->rows; i++)
        for(int j=0; j<u->cols; j++)
        {
            h->at<double>(0, u->at<uchar>(i,j)) += 1;
        }

}

void getMinMax(cv::Mat* hist, int *min, int*max)
{
    *min=0;
    *max=255;
    for (int i=0; i<256; i++)
    {
        if (hist->at<double>(0,i)!=0)
        {
            *min = i;
            break;
        }
    }

    for (int i=255; i>-1; i--)
    {
        if (hist->at<double>(0,i)!=0)
        {
            *max = i;
            break;
        }
    }
}
