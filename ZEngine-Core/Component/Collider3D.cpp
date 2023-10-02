#include "Collider3D.h"
#include "RigidBody3D.h"
#include "../Map/Objects/Entity.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

Collider3D::Collider3D(std::string name, ObjectType objectType) : Component(name, objectType), _geometry(nullptr)
{
  RegisterDerivedType(COLLIDER_3D);
}

void Collider3D::SetGeometry(physx::PxGeometry* geometry)
{
  if (GetGeometry() != nullptr) {
    auto geometry = GetGeometry();
    delete geometry;
  }
  _geometry = geometry;

  // Try get the parent object.
  auto parent = GetOwner();
  if (parent == nullptr) return;

  // Notify the rigid body that the collider has been modified.
  auto rigidBody = parent->GetComponentByType<RigidBody3D>();
  if (rigidBody != nullptr) {
    rigidBody->OnColliderModified();
  }
}

physx::PxGeometry* Collider3D::GetGeometry() const
{
  return _geometry;
}

Collider3D::~Collider3D()
{
  if (_geometry != nullptr) delete _geometry;
}
