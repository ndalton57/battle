#pragma once

class Time
{
    static Time* instance;

    double m_dDeltaTime = 0.0;
    double m_dFrequency = 0.0;

    __int64 m_nStartTime = 0;
    __int64 m_nCurrentTime = 0;
    __int64 m_nPreviousTime = 0;

    Time();
    ~Time() = default;

public:
    static Time* getInstance();

    void Tick();
    double Current();
    double Delta() { return m_dDeltaTime; }
};