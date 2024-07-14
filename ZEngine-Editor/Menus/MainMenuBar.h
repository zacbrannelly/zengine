#pragma once
#include <ZEngine-Core/ImmediateUI/GUIElement.h>

class Editor;

class MainMenuBar : public GUIElement
{
public:
	MainMenuBar(Editor* editor);
	~MainMenuBar();

	void RenderElement() override;
	GUIElementType GetType() override;
private:
	Editor* _editor;
};

