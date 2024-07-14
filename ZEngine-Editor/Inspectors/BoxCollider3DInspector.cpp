#include "BoxCollider3DInspector.h"
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>

#include <ZEngine-Core/Component/BoxCollider3D.h>

BoxCollider3DInspector::BoxCollider3DInspector()
{
}

void BoxCollider3DInspector::RenderElement()
{
	if (GetValue() == nullptr) return;
	auto boxCollider = static_cast<BoxCollider3D*>(GetValue());

	ImGui::BeginChild(UniqueLabel("Box Collider 3D").c_str(), ImVec2(0, 40));
	{
		auto size = boxCollider->GetSize();
		float sizeArray[3] = { size.x, size.y, size.z };

		if (ImGui::DragFloat3("Size", sizeArray))
		{
			if (sizeArray[0] != 0 && sizeArray[1] != 0 && sizeArray[2] != 0)
			{
				boxCollider->SetSize({ sizeArray[0], sizeArray[1], sizeArray[2] });
			}
		}
	}
	ImGui::EndChild();
}

BoxCollider3DInspector::~BoxCollider3DInspector()
{
}
