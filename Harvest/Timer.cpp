#include "Timer.h"

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMillisecondsElapsed()
{
	if (m_isRunning)
	{
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
	
	return 0.0;
}

void Timer::Restart()
{
	m_isRunning = true;
	start = std::chrono::high_resolution_clock::now();
}

bool Timer::Start()
{
	if (m_isRunning)
		return false;
	
	start = std::chrono::high_resolution_clock::now();
	m_isRunning = true;

	return true;
}

bool Timer::Stop()
{
	if (!m_isRunning)
		return false;
	
	stop = std::chrono::high_resolution_clock::now();
	m_isRunning = false;

	return true;
}
