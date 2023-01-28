#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	uint32 ReadSec() const;
	float ReadMSec() const;
	void Pause();
	void Resume();
	void SetTime(float time);

private:
	uint32 startTime;
	uint32 savedTime = 0;
};

#endif //__TIMER_H__

