#include "BoxCollider3D.h"
#include "RigidBody3D.h"
#include "../Map/Objects/Entity.h"
#include "../Rendering/MeshFactory.h"
#include "../Rendering/MaterialFactory.h"
#include "../Rendering/Material.h"
#include "../Rendering/Mesh.h"
#include "../Component/Transform.h"
#include "../Component/MeshRenderer.h"
#include <iostream>

#define NDEBUG
#include <PxPhysicsAPI.h>

BoxCollider3D::BoxCollider3D() : DynamicCollider3D("Box Collider 3D", ObjectType::BOX_COLLIDER_3D), _size(1.0f)
{
  RegisterDerivedType(BOX_COLLIDER_3D);
    
  auto gizmoMesh = MeshFactory::CreateCubeOutline("BoxCollider3DGizmo");
  auto gizmoMaterial = MaterialFactory::CreateUnlitVertexColorMaterial("BoxCollider3DGizmoMaterial");
    
  auto colors = std::vector<glm::vec4>(gizmoMesh->GetVertices().size(), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  gizmoMesh->SetColors(colors);
    
  _cubeGizmoTransform = new Transform();
  _cubeGizmoRenderer = new MeshRenderer();
  _cubeGizmoRenderer->SetMesh(gizmoMesh);
  _cubeGizmoRenderer->SetMaterial(gizmoMaterial);
    
  SetSize(_size);
}

void BoxCollider3D::SetSize(glm::vec3 size)
{
  _size = size;

  auto geometry = new physx::PxBoxGeometry(size.x, size.y, size.z);
  SetGeometry(geometry);
    
  _cubeGizmoTransform->SetScale(_size);
}

glm::vec3 BoxCollider3D::GetSize() const
{
  return _size;
}

void BoxCollider3D::RenderGizmos(int viewId)
{
  auto parentTransform = GetOwner()->GetTransform();
  _cubeGizmoTransform->SetPosition(parentTransform->GetPosition());
  _cubeGizmoTransform->SetRotationQuaternion(parentTransform->GetRotationQuaternion());
  _cubeGizmoRenderer->Render(viewId, _cubeGizmoTransform);
}

ZObject* BoxCollider3D::CreateInstance(std::string name, ObjectType type)
{
  return new BoxCollider3D();
}

ZObject* BoxCollider3D::Copy(std::string name, ZObject* object)
{
  auto source = static_cast<BoxCollider3D*>(object);
  auto copy = new BoxCollider3D();

  copy->SetSize(source->GetSize());

  return copy;
}

BoxCollider3D::~BoxCollider3D()
{
  delete _cubeGizmoRenderer;
}
