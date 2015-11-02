#ifndef _TIMERLX_H_
#define _TIMERLX_H_

#include <Windows.h>

class TimerLx
{
public:
	TimerLx();
	~TimerLx();

	bool Initialize();
	double GetTime();
	void Frame();
	double GetFrameTime();

private:
	double _countsPerSecond;
	__int64 _counterStart;

	int _frameCount;
	int _fps;

	__int64 _frameTimeOld;
	double _frameTime;
};

#endif