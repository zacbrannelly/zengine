#include "SphereCollider3DInspector.h"
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>

#include <ZEngine-Core/Component/SphereCollider3D.h>

using namespace ZEngine;

SphereCollider3DInspector::SphereCollider3DInspector()
{
}

void SphereCollider3DInspector::RenderElement()
{
	if (GetValue() == nullptr) return;
	auto collider = static_cast<SphereCollider3D*>(GetValue());

	if (ImGui::BeginChild(UniqueLabel("Sphere Collider 3D").c_str(), ImVec2(0, 80)))
	{
		auto radius = collider->GetRadius();

		if (ImGui::DragFloat("Radius", &radius))
		{
			if (radius != 0)
			{
				collider->SetRadius(radius);
			}
		}

		auto isTrigger = collider->IsTrigger();
		if (ImGui::Checkbox("Is Trigger", &isTrigger))
		{
			collider->SetIsTrigger(isTrigger);
		}
	}
	ImGui::EndChild();
}

SphereCollider3DInspector::~SphereCollider3DInspector()
{
}
