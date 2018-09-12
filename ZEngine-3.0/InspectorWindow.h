#pragma once

#include "GUIWindow.h"

class Editor;

class InspectorWindow : public GUIWindow
{
public:
	InspectorWindow(Editor* context);
	~InspectorWindow();

	void ProcessInput() override;
	void RenderInWindow() override;

private:
	Editor* _context;
};

