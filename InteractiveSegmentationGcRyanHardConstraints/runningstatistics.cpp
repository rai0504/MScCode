// This is taken from John D. Cook

#include "runningstatistics.h"

RunningStatistics::RunningStatistics()
{
    m_n = 0;
}

void RunningStatistics::clear()
{
    m_n = 0;
}

void RunningStatistics::push(double x)
{
    m_n++;

    if (m_n==1)
    {
        m_oldM = x;
        m_newM = x;
        m_oldS = 0;
    }
    else
    {
        m_newM = m_oldM + (x-m_oldM)/m_n;
        m_newS = m_oldS + (x-m_oldM)*(x-m_newM);

        // set up for next iteration
        m_oldM = m_newM;
        m_oldS = m_newS;
    }
}

int RunningStatistics::numberOfDataValues()
{
    return m_n;
}

double RunningStatistics::mean()
{
    return (m_n>0) ? m_newM : 0.0;
}

double RunningStatistics::variance()
{
    return (m_n>1) ? m_newS/(m_n-1) : 0.0;
}

double RunningStatistics::standardDeviation()
{
    return std::sqrt(variance());
}
