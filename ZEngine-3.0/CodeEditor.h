#pragma once

#include "GUIWindow.h"

class CodeEditor : public GUIWindow
{
public:
	CodeEditor();
	~CodeEditor();

	void ProcessInput() override;
	void RenderInWindow() override;
};

