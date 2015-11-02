#include "TimerLx.h"


TimerLx::TimerLx()
{
	_countsPerSecond = 0.0f;
	_frameTimeOld = 0;
	_frameTime = 0.0f;
}


TimerLx::~TimerLx()
{
}

bool TimerLx::Initialize()
{
	LARGE_INTEGER frequencyCount;
	QueryPerformanceFrequency(&frequencyCount);

	_countsPerSecond = float(frequencyCount.QuadPart);

	return true;
}

void TimerLx::Frame()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - _frameTimeOld;
	_frameTimeOld = currentTime.QuadPart;

	if(tickCount < 0.0)
		tickCount = 0;

	_frameTime = float(tickCount) / _countsPerSecond;
}

float TimerLx::GetTime()
{
	return _frameTime;
}