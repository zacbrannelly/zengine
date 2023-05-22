#include "TransformInspector.h"
#include "../imgui-includes.h"

#include <ZEngine-Core/Component/Transform.h>

TransformInspector::TransformInspector()
{
}

void TransformInspector::RenderElement()
{
	if (GetValue() == nullptr) return;

	auto transform = static_cast<Transform*>(GetValue());

	ImGui::BeginChild("Transform", ImVec2(0, 100));
	{
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
}

TransformInspector::~TransformInspector()
{
}
