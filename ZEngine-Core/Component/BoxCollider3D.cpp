#include "BoxCollider3D.h"
#include "RigidBody3D.h"
#include "../Map/Objects/Entity.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

BoxCollider3D::BoxCollider3D() : Collider3D("Box Collider 3D", ObjectType::BOX_COLLIDER_3D)
{
  RegisterDerivedType(BOX_COLLIDER_3D);
  SetSize(glm::vec3(1.0f));
}

void BoxCollider3D::SetSize(glm::vec3 size)
{
  _size = size;

  if (GetGeometry() != nullptr) {
    auto geometry = GetGeometry();
    delete geometry;
  }

  auto geometry = new physx::PxBoxGeometry(size.x, size.y, size.z);
  SetGeometry(geometry);

  // Try get the parent object.
  auto parent = GetOwner();
  if (parent == nullptr) return;

  // Notify the rigid body that the collider has been modified.
  auto rigidBody = parent->GetComponentByType<RigidBody3D>();
  if (rigidBody != nullptr) {
    rigidBody->OnColliderModified();
  }
}

glm::vec3 BoxCollider3D::GetSize() const
{
  return _size;
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
}
