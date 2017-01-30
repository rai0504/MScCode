#ifndef RUNNINGSTATISTICS_H
#define RUNNINGSTATISTICS_H

#include <cmath>

class RunningStatistics
{
public:
    RunningStatistics();
    void clear();
    void push(double x);
    int numberOfDataValues();
    double mean();
    double variance();
    double standardDeviation();

private:
    int m_n;
    double m_oldM, m_newM, m_oldS, m_newS;
};

#endif // RUNNINGSTATISTICS_H
