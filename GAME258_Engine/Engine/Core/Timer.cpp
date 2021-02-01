#include "Timer.h"

//Set previous and current ticks to 0 on construction.
Timer::Timer() : prevTicks(0), currentTicks(0)
{
}

Timer::~Timer()
{
}

//Sets previous and current ticks to be the proper amount of ticks at timer start.
//SDL_GetTicks gets the number of milliseconds since SDL library was initialized.
void Timer::Start()
{
	prevTicks = currentTicks = SDL_GetTicks();
}

//Sets previous ticks to be whatever the current ticks are, then updated current ticks variable to be what the actual current ticks are at.
void Timer::UpdateFrameTicks()
{
	prevTicks = currentTicks;
	currentTicks = SDL_GetTicks();
}

//Gets time between ticks in seconds.
float Timer::GetDeltaTime() const
{
	return (currentTicks - prevTicks) / MILLI_TO_SEC;
}

/*This function returns the sleep time that will ensure the engine runs at the given fps_ by making the engine "sleep"
  for an amount of time equal to whatever it needs to be to ensure a consistent speed.*/
unsigned int Timer::GetSleepTime(const unsigned int fps_) const
{
	unsigned int milliSecsPerFrame = MILLI_TO_SEC / fps_;
	//No need for engine to "sleep"
	if (milliSecsPerFrame == 0)
	{
		return 0;
	}

	unsigned int sleepTime = milliSecsPerFrame - (SDL_GetTicks() - currentTicks);
	//Make sure engine "sleeps" for the shortest amount of time necessary.
	if (sleepTime > milliSecsPerFrame)
	{
		return milliSecsPerFrame;
	}
	else
	{
		return sleepTime;
	}
}

//Gets current amount of ticks in milliseconds as a float.
float Timer::GetCurrentTickMilli() const
{
	return static_cast<float>(currentTicks);
}

//Gets current amount of ticks in seconds as a float.
float Timer::GetCurrentTickSec() const
{
	return currentTicks / MILLI_TO_SEC;
}
