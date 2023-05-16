#pragma once

#include "GUIWindow.h"
#include "Inspector.h"
#include <ZEngine-Core/Map/Objects/ZObject.h>
#include <map>

class Editor;

class InspectorWindow : public GUIWindow
{
public:
	InspectorWindow(Editor* context);
	~InspectorWindow();

	void ProcessInput() override;
	void RenderInWindow() override;

private:
	Inspector* GetInspector(ObjectType type);

	Editor* _context;
	std::map<ObjectType, Inspector*> _inspectors;
};

