#pragma once
#include "GUIWindow.h"

class Editor;
class Entity;

class SceneGraphWindow : public GUIWindow
{
public:
	SceneGraphWindow(Editor* editor);
	~SceneGraphWindow();

	void ProcessInput() override;
	void RenderInWindow() override;

private:
	void RenderNode(Entity* node);

	Editor* _context;
};

