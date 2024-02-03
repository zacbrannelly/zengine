#include "CapsuleCollider3DInspector.h"
#include "../imgui-includes.h"

#include <ZEngine-Core/Component/CapsuleCollider3D.h>

CapsuleCollider3DInspector::CapsuleCollider3DInspector()
{
}

void CapsuleCollider3DInspector::RenderElement()
{
	if (GetValue() == nullptr) return;
	auto collider = static_cast<CapsuleCollider3D*>(GetValue());

	ImGui::BeginChild(UniqueLabel("Sphere Collider 3D").c_str(), ImVec2(0, 2 * 40));
	{
		auto radius = collider->GetRadius();
    auto height = collider->GetHeight();

		if (ImGui::DragFloat("Radius", &radius))
		{
			if (radius != 0)
			{
				collider->SetSize(radius, height);
			}
		}

    if (ImGui::DragFloat("Height", &height))
    {
      if (height != 0)
      {
        collider->SetSize(radius, height);
      }
    }
	}
	ImGui::EndChild();
}

CapsuleCollider3DInspector::~CapsuleCollider3DInspector()
{
}
