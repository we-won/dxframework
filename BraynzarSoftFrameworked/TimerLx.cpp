#include "TimerLx.h"


TimerLx::TimerLx()
{
	_countsPerSecond = 0.0f;
	_counterStart = 0;
	_frameCount = 0;
	_fps = 0;
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

	_countsPerSecond = double(frequencyCount.QuadPart);

	QueryPerformanceCounter(&frequencyCount);
	_counterStart = frequencyCount.QuadPart;

	return true;
}

double TimerLx::GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	
	return double(currentTime.QuadPart - _counterStart)/_countsPerSecond;
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

double TimerLx::GetFrameTime()
{
	return _frameTime;
}