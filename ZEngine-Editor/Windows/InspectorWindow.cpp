#include "InspectorWindow.h"
#include "../Editor.h"
#include <ZEngine-Core/Component/Transform.h>
#include "../imgui-includes.h"

#include "../Inspectors/TransformInspector.h"
#include "../Inspectors/CameraInspector.h"
#include "../Inspectors/MeshRendererInspector.h"

InspectorWindow::InspectorWindow(Editor* context) : GUIWindow("Inspector Window", 400, 1000, false)
{
	_context = context;
	
	// Register the inspectors for components here
	_inspectors[ObjectType::TRANSFORM] = new TransformInspector();
	_inspectors[ObjectType::CAMERA] = new CameraInspector();
	_inspectors[ObjectType::MESH_RENDERER] = new MeshRendererInspector();
}

Inspector* InspectorWindow::GetInspector(ObjectType type)
{
	return _inspectors.find(type) != _inspectors.end() ? _inspectors[type] : nullptr;
}

void InspectorWindow::ProcessInput()
{
}

void InspectorWindow::RenderInWindow()
{
	if (_context->GetSelectedEntity() != nullptr)
	{
		auto entity = _context->GetSelectedEntity();

		ImGui::Text(("Selected Entity: " + entity->GetName()).c_str());
		ImGui::Separator();
		
		for (auto component : entity->GetAllComponents())
		{
			auto inspectorElement = GetInspector(component->GetType());

			ImGui::TextColored(ImVec4(0, 1, 0, 1), component->GetName().c_str());

			if (inspectorElement != nullptr)
			{
				inspectorElement->Inspect(component);
				inspectorElement->RenderElement();
			}
			else
			{
				ImGui::Text("Can't inspect this component yet!");
			}

			ImGui::Separator();
		}
	}
}

InspectorWindow::~InspectorWindow()
{
}
