#include "TimerLx.h"


TimerLx::TimerLx()
{
	m_countsPerSecond = 0.0f;
	m_frameTimeOld = 0;
	m_frameTime = 0.0f;
}


TimerLx::~TimerLx()
{
}

bool TimerLx::Initialize()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	m_countsPerSecond = float(frequencyCount.QuadPart);

	return true;
}

void TimerLx::Frame()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - m_frameTimeOld;
	m_frameTimeOld = currentTime.QuadPart;

	if(tickCount < 0.0)
		tickCount = 0;

	m_frameTime = float(tickCount) / m_countsPerSecond;
}