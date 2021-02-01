#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

//1000 milliseconds in a second.
constexpr float MILLI_TO_SEC = 1000.0F;

class Timer
{
public:
	//Next 4 lines prevent use of copy or move constructor. Ensures only one instance of this timer will exist at any time.
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

	Timer();
	~Timer();
	
	void Start();
	void UpdateFrameTicks();
	float GetDeltaTime() const;
	unsigned int GetSleepTime(const unsigned int fps_) const;
	float GetCurrentTickMilli() const;
	float GetCurrentTickSec() const;

private:
	unsigned int prevTicks;
	unsigned int currentTicks;
};

#endif // !TIMER_H