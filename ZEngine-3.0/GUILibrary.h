#pragma once

#include <ZEngine-Core\Misc\Singleton.h>

class Display;

class GUILibrary : public Singleton<GUILibrary>
{
public:
	void Init(Display* display);
	void Shutdown();

	void NewFrame();
	void EndFrame();
};