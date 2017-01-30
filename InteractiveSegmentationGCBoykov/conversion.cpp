#include "conversion.h"

Graph *imageToGraph(cv::Mat *image, cv::Mat *seed, cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm, eInterConnectivity connectivity, double lambda, double sigma)
{
    static Graph g(image->rows, image->cols);
    if (sigma>0) setNWeights(&g, image, connectivity, lambda, sigma);
    else
    {
        RunningStatistics imageStats;
        calculatesImageStatistics(image, connectivity, &imageStats);
        setNWeights(&g, image, connectivity, lambda, imageStats.standardDeviation());
    }
    setTWeights(&g, image, seed, fgGmm, bgGmm);
    //cout << "MaxNFlow = " << maximumNeighbourhoodDissipation(&g) << endl;
    return &g;
}

// convert emgmm to probability histogram
void setTWeights(Graph *g, cv::Mat *image, cv::Mat *seed, cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm)
{
    cv::Mat probs;
    double dataWeights[2][256] = {0, };
    double K = 1+maximumNeighbourhoodDissipation(g);
    for (int i=0; i<256; i++)
    {
        dataWeights[FG][i] = -fgGmm->get()->predict2(i,probs)[0];
        dataWeights[BG][i] = -bgGmm->get()->predict2(i,probs)[0];
    }

    for (unsigned int r=0; r<g->r; r++)
        for (unsigned int c=0; c<g->c; c++)
        {
            if (seed->at<cv::Vec3b>(r,c)[0]==255)           // hard-constraint: background
            {
                g->addEdge(g->toIndex(r,c), g->t, K);
            }
            else if (seed->at<cv::Vec3b>(r,c)[2]==255)      // hard-constraint: foreground
            {
                g->addEdge(g->s, g->toIndex(r,c), K);
            }
            else
            {
                g->addEdge(g->toIndex(r, c), g->t, dataWeights[FG][image->at<cv::Vec3b>(r,c)[0]]);
                g->addEdge(g->s, g->toIndex(r, c), dataWeights[BG][image->at<cv::Vec3b>(r,c)[0]]);
            }
        }
}

double gaussianDifference(int intensity1, int intensity2, double sigma)
{
    return exp(-(pow(intensity1-intensity2,2))/(2*sigma*sigma));
}

void setNWeights(Graph *g, cv::Mat *image, eInterConnectivity connectivity, double lambda, double sigma)
{
    for (int r=0; r<g->r; r++)
        for (int c=0; c<g->c; c++)
        {
            if (connectivity==FOUR_WAY)
            {
                if (r>0)            g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), lambda*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r-1,c)[0], sigma)); // UP
                if (c<g->c-1)       g->addEdge(g->toIndex(r, c), g->toIndex(r, c+1), lambda*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r,c+1)[0], sigma)); // RIGHT
                if (r<g->r-1)       g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), lambda*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r+1,c)[0], sigma)); // BOTTOM
                if (c>0)            g->addEdge(g->toIndex(r, c), g->toIndex(r, c-1), lambda*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r,c-1)[0], sigma)); // LEFT
            }
            else if (connectivity==EIGHT_WAY)
            {
                if (r>0)
                {
                                    g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), lambda*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r-1,c)[0], sigma));            // UP
                    if (c<g->c-1)   g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), lambda*(1/sqrt(2))*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r-1,c+1)[0], sigma));   // UP-RIGHT
                    if (c>0)        g->addEdge(g->toIndex(r, c), g->toIndex(r-1, c), lambda*(1/sqrt(2))*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r-1,c-1)[0], sigma));   // UP-LEFT
                }
                if (c<g->c-1)       g->addEdge(g->toIndex(r, c), g->toIndex(r, c+1), lambda*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r,c+1)[0], sigma));            // RIGHT
                if (r<g->r-1)
                {
                                    g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), lambda*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r+1,c)[0], sigma));            // BOTTOM
                    if (c<g->c-1)   g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), lambda*(1/sqrt(2))*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r+1,c+1)[0], sigma));    // BOTTOM-RIGHT
                    if (c>0)        g->addEdge(g->toIndex(r, c), g->toIndex(r+1, c), lambda*(1/sqrt(2))*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r+1,c-1)[0], sigma));    // BOTTOM-LEFT
                }
                if (c>0)            g->addEdge(g->toIndex(r, c), g->toIndex(r, c-1), lambda*gaussianDifference(image->at<cv::Vec3b>(r,c)[0], image->at<cv::Vec3b>(r,c-1)[0], sigma));           // LEFT
            }
        }
}

double maximumNeighbourhoodDissipation(Graph *g)
{
    double maxNeighbourhoodFlow = 0;
    for(int r=0; r<g->r; r++)
        for(int c=0; c<g->c;c++)
        {
            double sum = 0;
            for(int e=0; e<g->node[r*g->c+c].edge.size(); e++)
            {
                sum += g->node[r*g->c+c].edge[e].capacity;
            }
            if (sum>maxNeighbourhoodFlow) maxNeighbourhoodFlow=sum;
        }
    return maxNeighbourhoodFlow;
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
