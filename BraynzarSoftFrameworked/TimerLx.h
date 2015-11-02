#ifndef _TIMERLX_H_
#define _TIMERLX_H_

#include <Windows.h>

class TimerLx
{
public:
	TimerLx();
	~TimerLx();

	bool Initialize();
	void Frame();
	float GetTime();

private:
	float _countsPerSecond;

	__int64 _frameTimeOld;
	float _frameTime;
};

#endif