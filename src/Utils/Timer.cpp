// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Utils/Timer.h"

#include <SDL_timer.h>

	
Timer::Timer()
{
	Start();
}

Timer::Timer(uint32_t startTime) {
	this->startTime = startTime;
}

void Timer::Start()
{
	startTime = SDL_GetTicks();
}

uint32_t Timer::ReadSec() const
{
	return (SDL_GetTicks() - startTime) / 1000;
}

float Timer::ReadMSec() const
{
	return (float)(SDL_GetTicks() - startTime);
}