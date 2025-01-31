#pragma once

#include <ZEngine-Core/ImmediateUI/GUIWindow.h>
#include "../Inspectors/Inspector.h"
#include <ZEngine-Core/Map/Objects/ZObject.h>
#include <map>

namespace ZEngine
{
	class Editor;
	class AddComponentPopup;

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
		AddComponentPopup* _addComponentPopup;
		std::map<ObjectType, Inspector*> _inspectors;
	};
}
