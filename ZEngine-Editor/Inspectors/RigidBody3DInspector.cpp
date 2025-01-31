#include "RigidBody3DInspector.h"
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>

#include <ZEngine-Core/Component/RigidBody3D.h>

using namespace ZEngine;

RigidBody3DInspector::RigidBody3DInspector()
{
}

void RigidBody3DInspector::RenderElement()
{
	if (GetValue() == nullptr) return;
	auto rigidBody = static_cast<RigidBody3D*>(GetValue());

	ImGui::BeginChild(UniqueLabel("Rigid Body 3D").c_str(), ImVec2(0, 210));
	{
    auto mass = rigidBody->GetMass();
		if (ImGui::DragFloat("Mass", &mass, 1, 0, 1000))
		{
			rigidBody->SetMass(mass);
		}

    auto linearDamping = rigidBody->GetLinearDamping();
		if (ImGui::DragFloat("Linear Damping", &linearDamping, 0.01f, 0, 1000))
    {
      rigidBody->SetLinearDamping(linearDamping);
    }

    auto angularDamping = rigidBody->GetAngularDamping();
    if (ImGui::DragFloat("Angular Damping", &angularDamping, 0.01f, 0, 1000))
    {
      rigidBody->SetAngularDamping(angularDamping);
    }

    auto useGravity = rigidBody->GetUseGravity();
    if (ImGui::Checkbox("Use Gravity", &useGravity))
    {
      rigidBody->SetUseGravity(useGravity);
    }

    auto isKinematic = rigidBody->GetIsKinematic();
    if (ImGui::Checkbox("Is Kinematic", &isKinematic))
    {
      rigidBody->SetIsKinematic(isKinematic);
    }

    if (isKinematic)
    {
      auto kinematicTarget = rigidBody->GetKinematicTarget();
      float target[3] = { kinematicTarget.x, kinematicTarget.y, kinematicTarget.z };
      if (ImGui::DragFloat3("Kinematic Target", target))
      {
        rigidBody->SetKinematicTarget({ target[0], target[1], target[2] });
      }
    }
	}
	ImGui::EndChild();
}

RigidBody3DInspector::~RigidBody3DInspector()
{
}
