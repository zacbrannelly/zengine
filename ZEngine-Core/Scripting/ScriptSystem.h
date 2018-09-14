#pragma once

#include "../Misc/Singleton.h"

class ScriptSystem : public Singleton<ScriptSystem>
{
public:
	ScriptSystem();
	~ScriptSystem();

	bool Init();
	void Shutdown();
};

