// ----------------------------------------------------
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "PerfTimer.h"

#include <SDL_timer.h>

uint64_t PerfTimer::frequency = 0;

PerfTimer::PerfTimer()
{
	Start();
}

void PerfTimer::Start()
{
	frequency = SDL_GetPerformanceFrequency();
	startTime = SDL_GetPerformanceCounter();
}

double PerfTimer::ReadMs() const
{
	return ((double)(SDL_GetPerformanceCounter() - startTime) / frequency * 1000);
}

uint64_t PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - startTime;
}
