#include "drawing.h"

#include "drawing.h"
#include "defines.h"

void plotDistributions(cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm)
{
    cv::Mat probs;
    cv::Mat distributionImage = cv::Mat::zeros(256*yDistributionScale, 256*xDistributionScale, CV_8UC3);
    double dist[2][256] ={0, };
    double max = 0;
    for (int i=0; i<256; i++)
    {
        if (fgGmm->get()->isTrained())
        {
            dist[0][i] = exp(fgGmm->get()->predict2(i,probs)[0]);
            if (dist[0][i] > max) max = dist[0][i];
        }
        if (bgGmm->get()->isTrained())
        {
            dist[1][i] = exp(bgGmm->get()->predict2(i,probs)[0]);
            if (dist[1][i] > max) max = dist[1][i];
        }
    }

    for (int c=1; c<256; c++)
    {
        cv::line(distributionImage,
                 cv::Point((c-1)*xDistributionScale, distributionImage.rows*(1-dist[0][c-1]/max)),
                 cv::Point((c)*xDistributionScale, distributionImage.rows*(1-dist[0][c]/max)),
                 fgMarker);
        cv::line(distributionImage,
                 cv::Point((c-1)*xDistributionScale, distributionImage.rows*(1-dist[1][c-1]/max)),
                 cv::Point((c)*xDistributionScale, distributionImage.rows*(1-dist[1][c]/max)),
                 bgMarker);
    }

    cv::imshow("Distribution", distributionImage);
}

void plotHistograms(cv::Mat *fgTrainingData, cv::Mat *bgTrainingData, cv::Ptr<cv::ml::EM> *fgGmm, cv::Ptr<cv::ml::EM> *bgGmm)
{
    cv::Mat probs;
    cv::Mat histImage = cv::Mat::zeros(200, 256, CV_8UC3);
    double hist[256] = {0, };
    double max;

    if (fgGmm->get()->isTrained())
    {
        /* draw FG Hist and estimation */
        for (int i=0; i<fgTrainingData->rows; i++) hist[(int)fgTrainingData->at<double>(i)] += 1;

        max = hist[0];
        double maxGmm = exp(fgGmm->get()->predict2(0,probs)[0]);
        for(int i=1; i<256; i++)
        {
            if (hist[i] > max) max = hist[i];
            if (exp(fgGmm->get()->predict2(i,probs)[0])>maxGmm) maxGmm = exp(fgGmm->get()->predict2(i,probs)[0]);
        }

        for (int i=0; i<256; i++)
        {
            cv::line(histImage,
                     cv::Point(i, 100),
                     cv::Point(i, 100-hist[i]/max*100),
                     fgMarker);
            //gmm
            if (i>0)
            {
                cv::line(histImage,
                         cv::Point(i, 100-exp(fgGmm->get()->predict2(i-1,probs)[0])/maxGmm*100),
                        cv::Point(i, 100-exp(fgGmm->get()->predict2(i,probs)[0])/maxGmm*100),
                        fgMarker);
            }
        }
    }

    if (bgGmm->get()->isTrained())
    {
        /* draw BG Hist and estimation */
        for (int i=0; i<256; i++) hist[i] = 0;

        for (int i=0; i<bgTrainingData->rows; i++)
        {
            hist[(int)bgTrainingData->at<double>(i)] += 1;
        }

        max = hist[0];
        double maxGmm = exp(bgGmm->get()->predict2(0,probs)[0]);
        for(int i=1; i<256; i++)
        {
            if (hist[i] > max) max = hist[i];
            if (exp(bgGmm->get()->predict2(i,probs)[0])>maxGmm) maxGmm = exp(bgGmm->get()->predict2(i,probs)[0]);
        }

        for (int i=0; i<256; i++)
        {
            // histogram
            cv::line(histImage,
                     cv::Point(i, 100),
                     cv::Point(i, 100+hist[i]/max*100),
                     bgMarker);

            //gmm
            if (i>0)
            {
                cv::line(histImage,
                         cv::Point(i, 100+exp(bgGmm->get()->predict2(i-1,probs)[0])/maxGmm*100),
                        cv::Point(i, 100+exp(bgGmm->get()->predict2(i,probs)[0])/maxGmm*100),
                        bgMarker);
            }
        }
    }

    cv::imshow("histogram", histImage);
}
