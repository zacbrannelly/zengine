#pragma once
#include "../UI/GUIElement.h"
class MainMenuBar : public GUIElement
{
public:
	MainMenuBar();
	~MainMenuBar();

	void RenderElement() override;
	GUIElementType GetType() override;
};
