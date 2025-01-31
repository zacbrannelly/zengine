#include "BoxCollider3D.h"
#include "RigidBody3D.h"
#include "../Map/Objects/Entity.h"
#include "../Component/Transform.h"
#include <iostream>

#define NDEBUG
#include <PxPhysicsAPI.h>

using namespace ZEngine;

BoxCollider3D::BoxCollider3D() : DynamicCollider3D("Box Collider 3D", ObjectType::BOX_COLLIDER_3D), _size(1.0f)
{
  RegisterDerivedType(BOX_COLLIDER_3D);
}

void BoxCollider3D::SetSize(glm::vec3 size)
{
  _size = size;
  OnGeometryChanged();
}

glm::vec3 BoxCollider3D::GetSize() const
{
  return _size;
}

glm::vec3 BoxCollider3D::GetWorldSize() const
{
  auto owner = GetOwner();
  if (owner == nullptr) return _size;

  auto worldSize = _size;
  auto transform = owner->GetTransform();
  auto scale = transform->GetWorldScale();

  worldSize *= scale;

  return worldSize;
}

void BoxCollider3D::BuildGeometry()
{
  auto owner = GetOwner();
  if (owner == nullptr) return;

  auto worldSize = GetWorldSize();
  auto geometry = new physx::PxBoxGeometry(worldSize.x, worldSize.y, worldSize.z);
  SetGeometrySilently(geometry);
}

ZObject* BoxCollider3D::CreateInstance(std::string name, ObjectType type)
{
  return new BoxCollider3D();
}

ZObject* BoxCollider3D::Copy(std::string name, ZObject* object)
{
  auto source = static_cast<BoxCollider3D*>(object);
  auto copy = new BoxCollider3D();

  copy->SetIsTrigger(source->IsTrigger());
  copy->SetSize(source->GetSize());

  return copy;
}

BoxCollider3D::~BoxCollider3D()
{
}
