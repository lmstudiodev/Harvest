#pragma once
#include <chrono>

class Timer
{
public:
	Timer();

public:
	double GetMillisecondsElapsed();
	void Restart();
	bool Start();
	bool Stop();

private:
	bool m_isRunning = false;

#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
#else
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> stop;
#endif // _WIN32

};