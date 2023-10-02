#include "InspectorWindow.h"
#include "../Editor.h"
#include <ZEngine-Core/Component/Transform.h>
#include "../imgui-includes.h"

#include "../Inspectors/TransformInspector.h"
#include "../Inspectors/CameraInspector.h"
#include "../Inspectors/MeshRendererInspector.h"
#include "../Inspectors/RigidBody3DInspector.h"
#include "../Inspectors/BoxCollider3DInspector.h"
#include "../Inspectors/SphereCollider3DInspector.h"
#include "../Inspectors/CapsuleCollider3DInspector.h"

#include <string>

InspectorWindow::InspectorWindow(Editor* context) : GUIWindow("Inspector Window", 400, 1000, false)
{
	_context = context;
	
	// Register the inspectors for components here
	_inspectors[TRANSFORM] = new TransformInspector();
	_inspectors[CAMERA] = new CameraInspector();
	_inspectors[MESH_RENDERER] = new MeshRendererInspector();
	_inspectors[RIGID_BODY_3D] = new RigidBody3DInspector();
	_inspectors[BOX_COLLIDER_3D] = new BoxCollider3DInspector();
	_inspectors[SPHERE_COLLIDER_3D] = new SphereCollider3DInspector();
	_inspectors[CAPSULE_COLLIDER_3D] = new CapsuleCollider3DInspector();
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
		
		auto deleteButtonIdx = 0;
		for (auto component : entity->GetAllComponents())
		{
			auto inspectorElement = GetInspector(component->GetType());

			ImGui::TextColored(ImVec4(1, 1, 1, 1), component->GetName().c_str());
			ImGui::SameLine();

			auto windowWidth = ImGui::GetWindowContentRegionWidth();
			auto removed = false;
			ImGui::SetCursorPosX(windowWidth - 28);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
			std::string deleteButtonName = std::string(ICON_FA_TRASH) + "###" + std::to_string(deleteButtonIdx++);
			if (ImGui::Button(deleteButtonName.c_str(), ImVec2(28, 28))) 
			{
				entity->RemoveComponent(component);
				inspectorElement = nullptr;
				removed = true;
			}
			ImGui::PopStyleVar();

			if (inspectorElement != nullptr)
			{
				inspectorElement->Inspect(component);
				inspectorElement->RenderElement();
			}
			else if (!removed)
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
