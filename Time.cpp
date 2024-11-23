#include "stdafx.h"
#include "Time.h"

Time* Time::instance = nullptr;
Time* gTime = Time::getInstance();

Time::Time()
{
    gTime = this;

    __int64 nFreq;
    QueryPerformanceFrequency((LARGE_INTEGER*)&nFreq);
    m_dFrequency = 1000.0 / (double)nFreq;

    QueryPerformanceCounter((LARGE_INTEGER*)&m_nStartTime);

    Tick();
}

Time* Time::getInstance()
{
    if (!instance)
        instance = new Time();

    return instance;
}

void Time::Tick()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);

    m_dDeltaTime = ((m_nCurrentTime - m_nPreviousTime) * m_dFrequency) / 1000.0;

    if (m_dDeltaTime > 0.1) // Delta cap for when the program is paused
        m_dDeltaTime = 0.1f;

    m_nPreviousTime = m_nCurrentTime;
}

double Time::Current()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&m_nCurrentTime);

    return (float)(((m_nCurrentTime - m_nStartTime) * m_dFrequency) / 1000.0f);
}