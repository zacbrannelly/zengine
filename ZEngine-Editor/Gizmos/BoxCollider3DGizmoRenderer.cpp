#include "BoxCollider3DGizmoRenderer.h"
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Component.h>
#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Component/BoxCollider3D.h>
#include <ZEngine-Core/Rendering/Mesh.h>
#include <ZEngine-Core/Rendering/Material.h>
#include <ZEngine-Core/Rendering/MeshFactory.h>
#include <ZEngine-Core/Rendering/MaterialFactory.h>

using namespace ZEngine;

BoxCollider3DGizmoRenderer::BoxCollider3DGizmoRenderer()
{
  _cubeOutlineMesh = MeshFactory::CreateCubeOutline("BoxCollider3DGizmo");
  _cubeOutlineMaterial = MaterialFactory::CreateUnlitVertexColorMaterial("BoxCollider3DGizmoMaterial");

  auto colors = std::vector<glm::vec4>(_cubeOutlineMesh->GetVertices().size(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  _cubeOutlineMesh->SetColors(colors);
}

void BoxCollider3DGizmoRenderer::Render(int viewId, const std::vector<Component*>& components)
{
  // TODO: Use instance rendering instead.
  for (Component* component : components)
  {
    auto collider = component->As<BoxCollider3D>();
    if (collider == nullptr) continue;

    auto transform = collider->GetOwner()->GetTransform()->GetWorldTransformMatrix();
    transform = glm::scale(transform, collider->GetSize());
    _cubeOutlineMesh->Draw(viewId, { _cubeOutlineMaterial }, transform); 
  }
}

BoxCollider3DGizmoRenderer::~BoxCollider3DGizmoRenderer()
{
  delete _cubeOutlineMesh;
  delete _cubeOutlineMaterial;
}
