#pragma once

#include "../UI/GUIWindow.h"

class LogWindow : public GUIWindow
{
public:
	LogWindow();
	~LogWindow();

	void ProcessInput() override;
	void RenderInWindow() override;
};

