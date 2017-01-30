#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

#include <iostream>
#include<vector>
using namespace std;

#include "defines.h"
#include "drawing.h"
#include "graph.h"
#include "conversion.h"
#include "pushrelabel.h"

void mouseEvent(int event, int x, int y, int flags, void* param);
void compositeImage();
void convertTo3ChannelGrayScale(cv::Mat* img, cv::Mat* output);
void seperateMask(cv::Mat *combinedMask, cv::Mat *fgMask, cv::Mat *bgMask);
cv::Mat getSamples(cv::Mat *hist);
void getContour(cv::Mat* binaryVariable, cv::Mat* contour);
void getCannyContour(cv::Mat* binaryVariable, cv::Mat* contour);
void drawContourOnImage(cv::Mat* img, cv::Mat* contour, cv::Mat* output);
void menu();

cv::Mat rawImage, u, maskImage, mouseLayer, compositedImage;

cv::Ptr<cv::ml::EM> fgGMM;
cv::Ptr<cv::ml::EM> bgGMM;

cv::Scalar activeDrawingColour = fgMarker;          // default the drawing colour to the foreground colour
eBrushElement brushElement = CIRCLE;
unsigned int brushSize = 10;                        // the size of the brush, default size to 10
unsigned int tempBrushSize = brushSize;             // used in resizing the brush
int mx = 0, my = 0;
bool change = true;

bool fgSeedAvailable = false;
bool bgSeedAvailable = false;

bool segmentationAvailable = false;

double lambda = 7.5;
double sigma = 5;

int main()
{
    // get image from user
    string rawImgPath;
    cout << "Path to image: ";
    cin >> rawImgPath;

    rawImage = cv::imread(rawImgPath, CV_LOAD_IMAGE_GRAYSCALE);
    if (rawImage.empty())
    {
        cout << "Error reading raw image..." << endl << "exiting..." << endl;
        return -1;
    }

    convertTo3ChannelGrayScale(&rawImage, &u);

    // get seed from user
    string seedPath;
    cout << "Path to seed [x - no seed]: ";
    cin >> seedPath;
    if (seedPath.compare("x")==0)
    {
        maskImage = cv::Mat::zeros(u.rows, u.cols, CV_8UC3);
    }
    else
    {
        maskImage = cv::imread(seedPath, CV_LOAD_IMAGE_COLOR);
    }
    if (maskImage.empty())
    {
        cout << "Error reading see image..." << endl << "exiting..." << endl;
        return -1;
    }

    compositedImage = cv::Mat::zeros(u.rows, u.cols, CV_8UC3);
    mouseLayer = cv::Mat::zeros(u.rows, u.cols, CV_8UC3);

    cvNamedWindow("Composite Image");

    cvSetMouseCallback(
                "Composite Image",
                mouseEvent,
                (void*)rawImage.data
                );

    cout << endl;
    menu();

    bool realTimeHistogram = false;
    string ans;
    cout << "Enable real time histogram [y/n]: ";
    cin >> ans;
    if (ans.compare("y")==0) realTimeHistogram = true;

    while (1)
    {
        compositeImage();
        cv::imshow("Composite Image", compositedImage);

        if (change)
        {
#if 1
            // get the foreground mask and background mask
            cv::Mat fgMask = cv::Mat::zeros(maskImage.rows, maskImage.cols, CV_8UC1); // used to calc the hist
            cv::Mat bgMask = cv::Mat::zeros(maskImage.rows, maskImage.cols, CV_8UC1); // used to calc the hist
            seperateMask(&maskImage,&fgMask, &bgMask);

            // use the foreground mask and get the foreground histogram (normalised)
            // use the background mask and get the background histogram (normalised)
            cv::Mat fgHist = cv::Mat::zeros(1, 256, CV_64F);
            cv::Mat bgHist = cv::Mat::zeros(1, 256, CV_64F);
            int histSize = 256;
            float range[] = {0, 256};
            const float *ranges[] = {range};
            int histChannels[] = {0}; // compute the histogram from the 0-th channel
            cv::calcHist(&rawImage, 1, histChannels, fgMask, fgHist, 1, &histSize, ranges, true, false);
            cv::calcHist(&rawImage, 1, histChannels, bgMask, bgHist, 1, &histSize, ranges, true, false);

            // EM Configuration for FG and BG Probabilities
            fgGMM = cv::ml::EM::create();
            fgGMM->setClustersNumber(number_of_clusters);
            fgGMM->setCovarianceMatrixType(cv::ml::EM::COV_MAT_DEFAULT);
            fgGMM->setTermCriteria(cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 300, 0.1));
            fgGMM->clear();

            bgGMM = cv::ml::EM::create();
            bgGMM->setClustersNumber(number_of_clusters);
            bgGMM->setCovarianceMatrixType(cv::ml::EM::COV_MAT_DEFAULT);
            bgGMM->setTermCriteria(cv::TermCriteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 300, 0.1));
            bgGMM->clear();

            // train the foreground gmm and background gmm
            cv::Mat fgTrainingData = getSamples(&fgHist);
            cv::Mat bgTrainingData = getSamples(&bgHist);
            if (fgSeedAvailable) fgGMM->trainEM(fgTrainingData);
            if (bgSeedAvailable) bgGMM->trainEM(bgTrainingData);

            // draw the GMM distributions and histograms
            //plotDistributions(&fgGMM, &bgGMM);
            if (realTimeHistogram) plotHistograms(&fgTrainingData, &bgTrainingData, &fgGMM, &bgGMM);
#endif
            change = false;
        }
        int keypressed = cv::waitKey(10);
        if (keypressed == ESC) break;
        else if (keypressed == 's' || keypressed == 'S')
        {
            cout << "Segmenting image --------------------------------" << endl;
            cout << "   generating graph...";
            //double lambda = 10.0;
            //double sigma = 2.5;
            Graph *g = imageToGraph(&u, &maskImage, &fgGMM, &bgGMM, EIGHT_WAY, lambda, sigma);
            cout << "complete" << endl;

            cout << "   finding minimum cut...";
            double t = (double)cv::getTickCount();
            PushRelabel pr(g);
            pr.calculateMaxFlow();
            t = ((double)cv::getTickCount()-t)/cv::getTickFrequency();
            cout << round(t/60) << "min " << ((int)t)%60 << "sec...complete" << endl;

            cout << "   highlighting segmented regions...";
            graphToImage(g, &maskImage);
            cout << "complete" << endl;
            cout << "Segmentation complete --------------------------------" << endl;

            segmentationAvailable = true;
        }
        else if (keypressed == 'm' || keypressed == 'M')
        {
            // save the mask image
            cout << endl << "Saving image mask..." << endl;
            string maskpath;
            cout << "Mask path: ";
            cin >> maskpath;
            cv::imwrite(maskpath, maskImage);
            cout << "complete" << endl;
        }

        else if (keypressed == 'c' || keypressed == 'C')
        {
            // save the composite image
            cout << endl << "Saving composite image...";
            string segpath;
            cout << "Segmentation path: ";
            cin >> segpath;
            cv::imwrite(segpath, compositedImage);
            cout << "complete" << endl;
        }
        else if (keypressed == 'r' || keypressed == 'R')
        {
            // reset the program
            fgSeedAvailable = false;
            bgSeedAvailable = false;
            maskImage = cv::Mat::zeros(rawImage.rows, rawImage.cols, CV_8UC3);
            fgGMM->clear();
            bgGMM->clear();
            //plotDistributions(&fgGMM, &bgGMM);
            cv::Mat empty;
            plotHistograms(&empty, &empty, &fgGMM, &bgGMM);
            change = false;
        }
    }

    //cv::waitKey();
    return 0;
}

void menu()
{
    cout << "Mouse Interaction" << endl;
    cout << "LMB + drag       - Mark a foreground/object seed" << endl;
    cout << "RMB + drag       - Mark a background seed" << endl;
    cout << "MMB + drag left  - Decrease the size of the paintbrush" << endl;
    cout << "MMB + drag right - Increase the size of the paintbrush" << endl;
    cout << "Move + Shift     - Eraser" << endl;
    cout << endl;
    cout << "Hot keys" << endl;
    cout << "s/S - Segment" << endl;
    cout << "m/M - Save mask/seed" << endl;
    cout << "c/C - Save composite image" << endl;
    cout << "r/R - Reset program" << endl;
    cout << "Esc - Exit" << endl;
    cout << endl << endl;
    return;
}

void mouseEvent(int event, int x, int y, int flags, void *param)
{
    switch (event)
    {
    case CV_EVENT_MOUSEMOVE:
    {
        if (flags==CV_EVENT_FLAG_LBUTTON)
        {
            change = true;
            if (brushElement==RECTANGLE)
            {
                cv::rectangle(maskImage,
                              cv::Point(x-brushSize, y-brushSize),
                              cv::Point(x+brushSize, y+brushSize),
                              fgMarker,
                              CV_FILLED);
            }
            else if (brushElement==CIRCLE)
            {
                cv::circle(maskImage,
                              cv::Point(x, y),
                              brushSize,
                              fgMarker,
                              CV_FILLED);
            }
        }
        else if (flags==CV_EVENT_FLAG_RBUTTON)
        {
            change = true;
            if (brushElement==RECTANGLE)
            {
                cv::rectangle(maskImage,
                              cv::Point(x-brushSize, y-brushSize),
                              cv::Point(x+brushSize, y+brushSize),
                              bgMarker,
                              CV_FILLED);
            }
            else if (brushElement==CIRCLE)
            {
                cv::circle(maskImage,
                              cv::Point(x, y),
                              brushSize,
                              bgMarker,
                              CV_FILLED);
            }
        }
        else if (flags==CV_EVENT_FLAG_MBUTTON)
        {
            mouseLayer = cv::Mat::zeros(u.rows, u.cols, CV_8UC3);
            if (brushSize+x-mx > 0 && brushSize+x-mx < maxBrushSize)
            {
                tempBrushSize = brushSize + x - mx;
            }
            if (brushElement==RECTANGLE)
            {
                cv::rectangle(mouseLayer,
                              cv::Point(mx-tempBrushSize, my-tempBrushSize),
                              cv::Point(mx+tempBrushSize, my+tempBrushSize),
                              activeDrawingColour,
                              CV_FILLED);
            }
            else if (brushElement==CIRCLE)
            {
                cv::circle(mouseLayer,
                              cv::Point(mx, my),
                             tempBrushSize,
                              activeDrawingColour,
                              CV_FILLED);
            }
        }
        else if (flags==CV_EVENT_FLAG_SHIFTKEY)
        {
            change = true;
            if (brushElement==RECTANGLE)
            {
                cv::rectangle(maskImage,
                              cv::Point(x-brushSize, y-brushSize),
                              cv::Point(x+brushSize, y+brushSize),
                              eraseMarker,
                              CV_FILLED);
                cv::rectangle(mouseLayer,
                              cv::Point(x-brushSize, y-brushSize),
                              cv::Point(x+brushSize, y+brushSize),
                              cv::Scalar(255, 255, 255),
                              CV_FILLED);
            }
            else if (brushElement==CIRCLE)
            {
                cv::circle(maskImage,
                              cv::Point(x, y),
                              brushSize,
                              eraseMarker,
                              CV_FILLED);
                cv::circle(mouseLayer,
                              cv::Point(x, y),
                              brushSize,
                              cv::Scalar(255, 255, 255),
                              CV_FILLED);
            }
        }
        else
        {
            mouseLayer = cv::Mat::zeros(u.rows, u.cols, CV_8UC3);
            if (brushElement==RECTANGLE)
            {
                cv::rectangle(mouseLayer,
                              cv::Point(x-brushSize, y-brushSize),
                              cv::Point(x+brushSize, y+brushSize),
                              activeDrawingColour,
                              CV_FILLED);
            }
            else if (brushElement==CIRCLE)
            {
                cv::circle(mouseLayer,
                              cv::Point(x, y),
                              brushSize,
                              activeDrawingColour,
                              CV_FILLED);
            }
        }
        break;
    }
    case CV_EVENT_LBUTTONDOWN:
    {
        activeDrawingColour = fgMarker;
        if (brushElement==RECTANGLE)
        {
            cv::rectangle(mouseLayer,
                          cv::Point(x-brushSize, y-brushSize),
                          cv::Point(x+brushSize, y+brushSize),
                          fgMarker,
                          CV_FILLED);
        }
        else if (brushElement==CIRCLE)
        {
            cv::circle(mouseLayer,
                          cv::Point(x-brushSize, y-brushSize),
                          brushSize,
                          fgMarker,
                          CV_FILLED);
        }
        break;
    }
    case CV_EVENT_LBUTTONUP:
    {
        break;
    }
    case CV_EVENT_RBUTTONDOWN:
    {
        activeDrawingColour = bgMarker;
        if (brushElement==RECTANGLE)
        {
            cv::rectangle(mouseLayer,
                          cv::Point(x-brushSize, y-brushSize),
                          cv::Point(x+brushSize, y+brushSize),
                          bgMarker,
                          CV_FILLED);
        }
         else if (brushElement==CIRCLE)
        {
            cv::circle(mouseLayer,
                          cv::Point(x-brushSize, y-brushSize),
                          brushSize,
                          bgMarker,
                          CV_FILLED);
        }
        break;
    }
    case CV_EVENT_RBUTTONUP:
    {
        break;
    }
    case CV_EVENT_MBUTTONDOWN:
    {
        // enable brush resizing
        mx = x;
        my = y;
        break;
    }
    case CV_EVENT_MBUTTONUP:
    {
        // disable brush resizing
        mx = 0;
        my = 0;
        brushSize = tempBrushSize;
        break;
    }
    }
}

void compositeImage()
{
    if (!segmentationAvailable)
    {
        cv::addWeighted(u, 1.0, maskImage, 1.0, 0.0, compositedImage);
        cv::addWeighted(compositedImage, 1.0, mouseLayer, 1.0, 0.0, compositedImage);
    }
    else
    {
        if (saveBinaryVariableState)
        {
            cv::Mat binMask = cv::Mat::zeros(u.rows, u.cols, CV_8UC1);
            for(int r=0; r<maskImage.rows; r++)
                for(int c=0; c<maskImage.cols; c++)
                    if (maskImage.at<cv::Vec3b>(r,c)[2]==255) binMask.at<uchar>(r,c)=255;
            cv::Mat contour;
            getContour(&binMask, &contour);
            //getCannyContour(&binMask, &contour);

            cv::imshow("binmask", binMask);
            cv::imshow("contour",contour);
        }

        cv::Mat tempMask = maskImage;
        for(int r=0; r<tempMask.rows; r++)
            for(int c=0; c<tempMask.cols;c++)
                tempMask.at<cv::Vec3b>(r,c)[0] = 0;
        cv::addWeighted(u, 1.0, tempMask, 1.0, 0.0, compositedImage);
    }
}

void convertTo3ChannelGrayScale(cv::Mat* img, cv::Mat* output)
{
    cv::Mat channel[3];
    img->copyTo(channel[0]);
    img->copyTo(channel[1]);
    img->copyTo(channel[2]);
    cv::merge(channel, 3, *output);
}

void seperateMask(cv::Mat *combinedMask, cv::Mat *fgMask, cv::Mat *bgMask)
{
    fgSeedAvailable = false;
    bgSeedAvailable = false;
    for (int r=0; r<combinedMask->rows; r++)
        for (int c=0; c<combinedMask->cols; c++)
        {
            if (combinedMask->at<cv::Vec3b>(r,c)[0] == 255)
            {
                bgMask->at<uchar>(r,c)=255;
                bgSeedAvailable = true;
            }
            else if (combinedMask->at<cv::Vec3b>(r,c)[2] == 255)
            {
                fgMask->at<uchar>(r,c)=255;
                fgSeedAvailable = true;
            }
        }
}

cv::Mat getSamples(cv::Mat *hist)
{
    double sum = cv::sum(*hist)[0];

    if ((int)sum == 0) return cv::Mat();

    float *histData = (float*)hist->ptr<float>(0);

    cv::Mat samples = cv::Mat::zeros((int)sum, 1, CV_64FC1);
    double* data = samples.ptr<double>(0);

    int index = 0;
    for (int i=0; i<256; i++)
    {
        for (int j=0; j<(int)histData[i]; j++)
        {
            data[index] = (double)i;
            index++;
        }
    }

    return samples;
}

void getContour(cv::Mat* binaryVariable, cv::Mat* contour)
{
    int apertureSize = 3; // determines the thickness of the contour
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

void drawContourOnImage(cv::Mat* img, cv::Mat* contour, cv::Mat* output)
{
    cv::Mat splitchannel[3];
    cv::split(*img, splitchannel);

    cv::Mat inverseContour = cv::Mat::ones(img->rows, img->cols, CV_8UC1);
    inverseContour = inverseContour - *contour;
    splitchannel[0] = splitchannel[0].mul(inverseContour);
    splitchannel[0].copyTo(splitchannel[1]);
    splitchannel[0].copyTo(splitchannel[2]);

    cv::Mat temp2;
    contour->copyTo(temp2);
    temp2 *= 255;
    splitchannel[2] = splitchannel[2]+temp2;

    cv::merge(splitchannel, 3, *output);
}
