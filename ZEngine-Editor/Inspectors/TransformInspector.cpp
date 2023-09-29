#include "TransformInspector.h"
#include "../imgui-includes.h"

#include <ZEngine-Core/Component/Transform.h>
#include <iostream>

TransformInspector::TransformInspector() : _initialized(false), _lastComponentInspected(nullptr)
{
}

void TransformInspector::RenderElement()
{
	if (GetValue() == nullptr) return;
	auto transform = static_cast<Transform*>(GetValue());

	if (_lastComponentInspected != transform) {
		_rotationEulerDegrees = transform->GetRotation();
		_lastComponentInspected = transform;
	}

	ImGui::BeginChild("Transform", ImVec2(0, 120));
	{
		float pos[3] = { transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z };
		float rot[3] = { _rotationEulerDegrees.x, _rotationEulerDegrees.y, _rotationEulerDegrees.z };
		float scale[3] = { transform->GetScale().x, transform->GetScale().y, transform->GetScale().z };

		if (ImGui::DragFloat3("Position", pos))
		{
			transform->SetPosition({ pos[0], pos[1], pos[2] });
		}

		if (ImGui::DragFloat3("Rotation", rot))
		{
			_rotationEulerDegrees = { rot[0], rot[1], rot[2] };
			transform->SetRotation({ rot[0], rot[1], rot[2] });
		}
 
		if (!ImGui::IsItemActive())
		{
			_rotationEulerDegrees = transform->GetRotation();
		}

		if (ImGui::DragFloat3("Scale", scale))
		{
			transform->SetScale({ scale[0], scale[1], scale[2] });
		}
	}
	ImGui::EndChild();
}

TransformInspector::~TransformInspector()
{
}
