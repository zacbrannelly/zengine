#include "SphereCollider3DGizmoRenderer.h"
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Component.h>
#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Component/SphereCollider3D.h>
#include <ZEngine-Core/Rendering/Mesh.h>
#include <ZEngine-Core/Rendering/Material.h>
#include <ZEngine-Core/Rendering/MeshFactory.h>
#include <ZEngine-Core/Rendering/MaterialFactory.h>

SphereCollider3DGizmoRenderer::SphereCollider3DGizmoRenderer()
{
  _outlineMesh = MeshFactory::CreateSphereOutline("SphereCollider3DGizmo", 20, 5);
  _outlineMaterial = MaterialFactory::CreateUnlitVertexColorMaterial("SphereCollider3DGizmoMaterial");

  auto colors = std::vector<glm::vec4>(_outlineMesh->GetVertices().size(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  _outlineMesh->SetColors(colors);
}

void SphereCollider3DGizmoRenderer::Render(int viewId, const std::vector<Component*>& components)
{
  // TODO: Use instance rendering instead.
  for (Component* component : components)
  {
    auto collider = component->As<SphereCollider3D>();
    if (collider == nullptr) continue;

    auto position = collider->GetOwner()->GetTransform()->GetWorldPosition();
    auto rotation = collider->GetOwner()->GetTransform()->GetWorldRotationQuaternion();
    auto transform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation);

    // Scale according to the collider's radius and add a small offset to avoid z-fighting.
    transform = glm::scale(transform, glm::vec3(collider->GetWorldRadius(), collider->GetWorldRadius(), collider->GetWorldRadius()));
    transform = glm::scale(transform, glm::vec3(1.01f, 1.01f, 1.01f));

    _outlineMesh->Draw(viewId, { _outlineMaterial }, transform); 
  }
}

SphereCollider3DGizmoRenderer::~SphereCollider3DGizmoRenderer()
{
  delete _outlineMesh;
  delete _outlineMaterial;
}
