#include "Time.h"

#include <chrono>

using namespace std::chrono;
using namespace ZEngine;

void Time::Init()
{
	_fps = 0;
	_deltaTime = 0;

	_timeStarted = duration_cast<std::chrono::seconds>(high_resolution_clock::now().time_since_epoch()).count();
}

void Time::Tick()
{
	_deltaTime = GetTime() - _lastTick;
	_lastTick = GetTime();

	_fps = 1.0 / _deltaTime;
}

double Time::GetTime() const
{
	return duration_cast<std::chrono::duration<double>>(high_resolution_clock::now().time_since_epoch()).count() - GetStartTime();
}

double Time::GetStartTime() const
{
	return _timeStarted;
}

double Time::GetFPS() const
{
	return _fps;
}

double Time::GetDeltaTime() const
{
	return _deltaTime;
}

void Time::Shutdown()
{

}

