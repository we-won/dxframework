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

	float GetTime() { return m_frameTime; }

private:
	float m_countsPerSecond;

	__int64 m_frameTimeOld;
	float m_frameTime;
};

#endif