#pragma once
#include "../Misc/Singleton.h"

class Time : public Singleton<Time>
{
public:
	void Init();
	void Shutdown();

	void Tick();

	double GetStartTime() const;
	double GetTime() const;
	double GetFPS() const;
	double GetDeltaTime() const;

private:
	double _fps;
	double _deltaTime;
	double _timeStarted;
	double _lastTick;
};

