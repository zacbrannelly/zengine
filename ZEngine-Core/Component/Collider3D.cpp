#include "Collider3D.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

Collider3D::Collider3D(std::string name, ObjectType objectType) : Component(name, objectType), _geometry(nullptr)
{
  RegisterDerivedType(COLLIDER_3D);
}

void Collider3D::SetGeometry(physx::PxGeometry* geometry)
{
  _geometry = geometry;
}

physx::PxGeometry* Collider3D::GetGeometry() const
{
  return _geometry;
}

Collider3D::~Collider3D()
{
  if (_geometry != nullptr) delete _geometry;
}
