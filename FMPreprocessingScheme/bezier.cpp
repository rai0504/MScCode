#include "bezier.h"

double quadraticSolve(double a, double b, double c, double *x1, double *x2)
{
    double delta = b*b-4*a*c;
    *x1 = (-b+sqrt(delta))/(2*a);
    *x2 = (-b-sqrt(delta))/(2*a);
    return delta;
}

void bezierCurve(Point p0, Point p1, Point p2, cv::Mat *curve)
{
    cout << "p0x = " << p0.x << ", p1.x = " << p1.x << ", p2.x = " << p2.x << endl;
    if ((p0.x<p1.x) && (p1.x<p2.x) )
    {
        *curve = cv::Mat::zeros(2, p2.x-p0.x, CV_64FC1);
    }

    int count = 0;
    for (int bx=p0.x; bx<p2.x; bx++)
    {
        double a = p0.x-2*p1.x+p2.x;
        double b = -2*p0.x + 2*p1.x;
        double c = p0.x - bx;
        double t1, t2;
        double delta = quadraticSolve(a, b, c, &t1, &t2);

        double t=0;
        if (t1>=0 && t1<=1) t=t1;
        else t=t2;

        // calculate corresponding by for given bx which occurs at t
        double by = pow(1-t, 2)*p0.y + 2*(1-t)*t*p1.y + t*t*p2.y;
        //cout << "a,b,c = " << a << "," << b << "," << c << ", delta = " << delta << ", t = " << t << ", Bx = " << bx << ", By = " << by << endl;

        if (by<0) by=0;
        if (by>255) by=255;
#if 0
        if (bx%2==0) cout << endl;
        cout << "Bx = " << bx << ", By = " << by;
        cout << "\t";
#endif

        curve->at<double>(0, count)=bx;
        curve->at<double>(1, count)=by;
        count++;
    }
    curve->at<double>(0,1) = p0.y;
    curve->at<double>(1,p2.x-p0.x-1) = p2.y;
    return;
}

void concatenateBezierCurve(Point p0, Point p2, Point p4, cv::Mat *attCurve, cv::Mat *ampCurve, cv::Mat *finalCurve)
{
    *finalCurve = cv::Mat::zeros(2,256,CV_64FC1);
    for(int i=0; i<p0.x; i++) finalCurve->at<double>(0,i)=i;
    for(int i=p0.x; i<p2.x; i++)
    {
        finalCurve->at<double>(0,i)=i;
        finalCurve->at<double>(1,i)=round(attCurve->at<double>(1,i-p0.x));
    }
    for(int i=p2.x; i<p4.x; i++)
    {
        finalCurve->at<double>(0,i)=i;
        finalCurve->at<double>(1,i)=round(ampCurve->at<double>(1,i-p2.x));
    }
    for (int i=p4.x; i<256; i++)
    {
        finalCurve->at<double>(0,i) = i;
        finalCurve->at<double>(1,i) = 255;
    }
}
