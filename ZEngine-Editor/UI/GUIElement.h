#pragma once

enum GUIElementType
{
	GUI_TYPE_ELEMENT,
	GUI_TYPE_CONTAINER,
	GUI_TYPE_WINDOW,
	GUI_TYPE_IMAGE,
	GUI_TYPE_MAIN_MENU_BAR,
	GUI_TYPE_INSPECTOR,
	GUI_TYPE_CODE_EDITOR,
	GUI_TYPE_TEXTFIELD,
	GUI_TYPE_SHADER_EDITOR,
	GUI_TYPE_MATERIAL_EDITOR,
	GUI_TYPE_CONTROLLER
};

class GUIElement
{
public:
	virtual void Update() {}
	virtual void RenderElement() = 0;
	virtual GUIElementType GetType() = 0;
};

