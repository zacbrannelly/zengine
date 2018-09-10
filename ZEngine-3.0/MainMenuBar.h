#pragma once
#include "GUIElement.h"
class MainMenuBar : public GUIElement
{
public:
	MainMenuBar();
	~MainMenuBar();

	void RenderElement() override;
};

