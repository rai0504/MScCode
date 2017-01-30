#include "histogram.h"

cv::Mat window1(window_height, window_width, CV_8UC3, cv::Scalar(0,0,0));

void plotHistogram(cv::Mat* histogram)
{
    cv::Mat histImage( histrogram_height, histogram_width, CV_8UC3, cv::Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    cv::normalize(*histogram, *histogram, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
    //normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    //normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    /// Draw for each channel
    for( int i = 1; i < number_of_bins; i++ )
    {

    //cv::line( histImage, cv::Point( bin_width*(i-1), histrogram_height - (int)round(histogram->at<float>(i-1)) ) ,
    //        cv::Point( bin_width*(i), histrogram_height - (int)round(histogram->at<float>(i)) ),
    //        cv::Scalar( 255, 0, 0), 1, 8, 0  );
    /*
    cv::line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
            Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
            Scalar( 0, 255, 0), 2, 8, 0  );
    cv::line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
            Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
            Scalar( 0, 0, 255), 2, 8, 0  );
    */
    }

    /// Display
    cv::namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
    cv::imshow("calcHist Demo", histImage );
}

void plotLine(cv::String windowName, cv::Point startPoint, cv::Point endPoint, cv::Scalar color)
{
    //cv::Mat window1(window_height, window_width, CV_8UC3, cv::Scalar(0,0,0));

    if (windowName.compare("window1"))
    {
        double Sx = window_width/256;
        double Sy = window_height/256;
        //cv::line(window1, cv::Point((int)round(Sx*startPoint.x), (int)round(Sy*startPoint.y)),
        //         cv::Point((int)round(Sx*endPoint.x), (int)round(Sy*endPoint.y)), color, 1, 8, 0);
        //cv::namedWindow("Window 1", CV_WINDOW_AUTOSIZE );
        cv::imshow("Window 1", window1);
    }
    else cout << windowName << endl;
}
