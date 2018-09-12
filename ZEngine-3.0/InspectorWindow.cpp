#include "InspectorWindow.h"
#include "Editor.h"
#include <ZEngine-Core/Component/Transform.h>
#include "imgui-includes.h"

InspectorWindow::InspectorWindow(Editor* context) : GUIWindow("Inspector Window", 400, 1000, false)
{
	_context = context;
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

		// TODO: Move to another class
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Transform");
		ImGui::BeginChild("Transform", ImVec2(0, 100));
		{
			auto transform = entity->GetTransform();
			float pos[3] = { transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z };
			float rot[3] = { transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z };
			float scale[3] = { transform->GetScale().x, transform->GetScale().y, transform->GetScale().z };
			
			if (ImGui::DragFloat3("Position", pos))
			{
				transform->SetPosition({ pos[0], pos[1], pos[2] });
			}

			if (ImGui::DragFloat3("Rotation", rot))
			{
				transform->SetRotation({ rot[0], rot[1], rot[2] });
			}

			if (ImGui::DragFloat3("Scale", scale))
			{
				transform->SetScale({ scale[0], scale[1], scale[2] });
			}

			ImGui::Spacing();
		}
		ImGui::EndChild();

		ImGui::Separator();

		// TODO: Move to another class & actually implement
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Mesh Renderer");
		ImGui::BeginChild("MeshRenderer", ImVec2(0, 100));
		ImGui::Text("Derpinajjkjk");
		ImGui::Spacing();
		ImGui::EndChild();

	}
}

InspectorWindow::~InspectorWindow()
{
}
