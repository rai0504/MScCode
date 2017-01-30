#include "conversion.h"

Graph *imageToGraph(cv::Mat *image, cv::Mat *seed, cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm, eInterConnectivity connectivity, double lambda, double sigmaW, double sigmaR)
{
    static Graph g(image->rows, image->cols);
    setTWeights(&g, image, seed, fgGmm, bgGmm);
    if (sigmaW>0 && sigmaR>0) setNWeights(&g, image, connectivity, lambda, sigmaW, sigmaR);
    else
    {
        RunningStatistics imageStats;
        sigmaR = 1;
        calculatesImageStatistics(image, connectivity, &imageStats);
        setNWeights(&g, image, connectivity, lambda, imageStats.standardDeviation(), sigmaR);
    }
    return &g;
}

void setTWeights(Graph *g, cv::Mat *image, cv::Mat *seed, cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm)
{
    cv::Mat probs;
    double dataWeights[2][256] = {0, };
    for (int i=0; i<256; i++)
    {
        dataWeights[FG][i] = exp(fgGmm->get()->predict2(i,probs)[0])/(exp(fgGmm->get()->predict2(i,probs)[0])+exp(bgGmm->get()->predict2(i,probs)[0]));
        dataWeights[BG][i] = exp(bgGmm->get()->predict2(i,probs)[0])/(exp(fgGmm->get()->predict2(i,probs)[0])+exp(bgGmm->get()->predict2(i,probs)[0]));
    }

    for (unsigned int r=0; r<g->r; r++)
        for (unsigned int c=0; c<g->c; c++)
        {
            if (seed->at<cv::Vec3b>(r,c)[0]==255)           // hard-constraint: background
            {
                g->addEdge(g->toIndex(r,c), g->t, dataWeights[BG][image->at<cv::Vec3b>(r,c)[0]]);
            }
            else if (seed->at<cv::Vec3b>(r,c)[2]==255)      // hard-constraint: foreground
            {
                g->addEdge(g->s, g->toIndex(r,c), dataWeights[FG][image->at<cv::Vec3b>(r,c)[0]]);
            }
            else
            {
                g->addEdge(g->toIndex(r, c), g->t, dataWeights[BG][image->at<cv::Vec3b>(r,c)[0]]);
                g->addEdge(g->s, g->toIndex(r, c), dataWeights[FG][image->at<cv::Vec3b>(r,c)[0]]);
            }
        }
}

double gaussianDifference(int intensity1, int intensity2, double sigmaW)
{
    return exp(-(pow(intensity1-intensity2,2))/sigmaW);
}

void setNWeights(Graph *g, cv::Mat *image, eInterConnectivity connectivity, double lambda, double sigmaW, double sigmaR)
{
    for (int r=0; r<g->r; r++)
        for (int c=0; c<g->c; c++)
        {
            if (connectivity==FOUR_WAY)
            {
                if (r>0)            g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), lambda*exp(-1/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r-1,c)[0], sigmaW)); // UP
                if (c<g->c-1)       g->addEdge(g->toIndex(r, c), g->toIndex(r, c+1), lambda*exp(-1/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r,c+1)[0], sigmaW)); // RIGHT
                if (r<g->r-1)       g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), lambda*exp(-1/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r+1,c)[0], sigmaW)); // BOTTOM
                if (c>0)            g->addEdge(g->toIndex(r, c), g->toIndex(r, c-1), lambda*exp(-1/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r,c-1)[0], sigmaW)); // LEFT
            }
            else if (connectivity==EIGHT_WAY)
            {
                if (r>0)
                {
                                    g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), lambda*exp(-1/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r-1,c)[0], sigmaW));            // UP
                    if (c<g->c-1)   g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), lambda*exp(-sqrt(2)/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r-1,c+1)[0], sigmaW));   // UP-RIGHT
                    if (c>0)        g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), lambda*exp(-sqrt(2)/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r-1,c-1)[0], sigmaW));   // UP-LEFT
                }
                if (c<g->c-1)       g->addEdge(g->toIndex(r, c), g->toIndex(r, c+1), lambda*exp(-1/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r,c+1)[0], sigmaW));            // RIGHT
                if (r<g->r-1)
                {
                                    g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), lambda*exp(-1/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r+1,c)[0], sigmaW));            // BOTTOM
                    if (c<g->c-1)   g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), lambda*exp(-sqrt(2)/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r+1,c+1)[0], sigmaW));    // BOTTOM-RIGHT
                    if (c>0)        g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), lambda*exp(-sqrt(2)/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r+1,c-1)[0], sigmaW));    // BOTTOM-LEFT
                }
                if (c>0)            g->addEdge(g->toIndex(r, c), g->toIndex(r, c-1), lambda*exp(-1/sigmaR)*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r,c-1)[0], sigmaW));           // LEFT
            }
        }
}

void calculatesImageStatistics(cv::Mat *image, eInterConnectivity connectivity, RunningStatistics *imageStats)
{
    int diff = 0;
    imageStats->clear();
    for (int r = 0; r<image->rows; r++)
        for (int c=0; c<image->cols; c++)
        {
            if (connectivity==EIGHT_WAY)
            {
                if (c<image->cols-1)
                {
                    diff = abs(image->at<cv::Vec3b>(r,c)[0] - image->at<cv::Vec3b>(r,c+1)[0]);
                    imageStats->push(diff);

                    if (r<image->rows-1)
                    {
                        diff = abs(image->at<cv::Vec3b>(r,c)[0] - image->at<cv::Vec3b>(r+1,c+1)[0]);
                        imageStats->push(diff);
                    }
                }
                if (r<image->rows-1)
                {
                    diff = abs(image->at<cv::Vec3b>(r,c)[0] - image->at<cv::Vec3b>(r+1,c)[0]);
                    imageStats->push(diff);
                }
                if (c>0)
                {
                    diff = abs(image->at<cv::Vec3b>(r,c)[0] - image->at<cv::Vec3b>(r+1,c-1)[0]);
                    imageStats->push(diff);
                }
            }
        }
}

void graphToImage(Graph *g, cv::Mat *segmentationMask)
{
    for(int r=0; r<g->r; r++)
        for(int c=0; c<g->c; c++)
        {
            if (g->node[g->toIndex(r, c)].distance>=g->r*g->c+2)
            {
                segmentationMask->at<cv::Vec3b>(r, c)[0] = 0;
                segmentationMask->at<cv::Vec3b>(r, c)[1] = 0;
                segmentationMask->at<cv::Vec3b>(r, c)[2] = 255;
            }
            else
            {
                segmentationMask->at<cv::Vec3b>(r, c)[0] = 255;
                segmentationMask->at<cv::Vec3b>(r, c)[1] = 0;
                segmentationMask->at<cv::Vec3b>(r, c)[2] = 0;
            }
        }

}
