#include "CapsuleCollider3DGizmoRenderer.h"
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Component.h>
#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Component/CapsuleCollider3D.h>
#include <ZEngine-Core/Rendering/Mesh.h>
#include <ZEngine-Core/Rendering/Material.h>
#include <ZEngine-Core/Rendering/MeshFactory.h>
#include <ZEngine-Core/Rendering/MaterialFactory.h>

#include <iostream>

using namespace ZEngine;

CapsuleCollider3DGizmoRenderer::CapsuleCollider3DGizmoRenderer()
{
  _hemiSphereOutlineMesh = MeshFactory::CreateHemiSphereOutline("CapsuleCollider3DGizmo", 20, 5);
  _cylinderOutlineMesh = MeshFactory::CreateCylinderOutline("CapsuleCollider3DGizmo", 20, 1);
  _outlineMaterial = MaterialFactory::CreateUnlitVertexColorMaterial("CapsuleCollider3DGizmoMaterial");

  auto colors = std::vector<glm::vec4>(_hemiSphereOutlineMesh->GetVertices().size(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  _hemiSphereOutlineMesh->SetColors(colors);

  colors = std::vector<glm::vec4>(_cylinderOutlineMesh->GetVertices().size(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  _cylinderOutlineMesh->SetColors(colors);
}

void CapsuleCollider3DGizmoRenderer::Render(int viewId, const std::vector<Component*>& components)
{
  // TODO: Use instance rendering instead.
  for (Component* component : components)
  {
    auto collider = component->As<CapsuleCollider3D>();
    if (collider == nullptr) continue;

    auto position = collider->GetOwner()->GetTransform()->GetWorldPosition();
    auto rotation = collider->GetOwner()->GetTransform()->GetWorldRotationQuaternion();
    auto transform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation);

    glm::mat4 cylinderTransform(1.0f);
    glm::mat4 topHemiSphereTransform(1.0f);
    glm::mat4 bottomHemiSphereTransform(1.0f);

    // Position the top and bottom hemispheres at the top and bottom of the capsule.
    auto height = collider->GetWorldHeight();
    auto radius = collider->GetWorldRadius();

    // Rotate everything 90 degrees on the Z axis relative to the capsule's origin (since capsules are along the X-axis).
    cylinderTransform = glm::rotate(cylinderTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    topHemiSphereTransform = glm::rotate(topHemiSphereTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    bottomHemiSphereTransform = glm::rotate(bottomHemiSphereTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    // Scale the cylinder and hemispheres to match the capsule's height and radius.
    cylinderTransform = glm::scale(cylinderTransform, glm::vec3(radius, height, radius));
    topHemiSphereTransform = glm::scale(topHemiSphereTransform, glm::vec3(radius, radius, radius));
    bottomHemiSphereTransform = glm::scale(bottomHemiSphereTransform, glm::vec3(radius, radius, radius));

    // Translate the hemispheres to the top and bottom of the cylinder.
    topHemiSphereTransform = glm::translate(topHemiSphereTransform, glm::vec3(0.0f, height, 0.0f));
    bottomHemiSphereTransform = glm::translate(bottomHemiSphereTransform, glm::vec3(0.0f, -height, 0.0f));
    bottomHemiSphereTransform = glm::rotate(bottomHemiSphereTransform, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // Apply the capsule's transform to the cylinder and hemispheres.
    cylinderTransform = transform * cylinderTransform;
    topHemiSphereTransform = transform * topHemiSphereTransform;
    bottomHemiSphereTransform = transform * bottomHemiSphereTransform;

    _cylinderOutlineMesh->Draw(viewId, { _outlineMaterial }, cylinderTransform);
    _hemiSphereOutlineMesh->Draw(viewId, { _outlineMaterial }, topHemiSphereTransform);
    _hemiSphereOutlineMesh->Draw(viewId, { _outlineMaterial }, bottomHemiSphereTransform);
  }
}

CapsuleCollider3DGizmoRenderer::~CapsuleCollider3DGizmoRenderer()
{
  delete _hemiSphereOutlineMesh;
  delete _cylinderOutlineMesh;
  delete _outlineMaterial;
}
