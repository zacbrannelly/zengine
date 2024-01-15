#include "PlaneCollider3D.h"
#include "RigidBody3D.h"
#include "../Map/Objects/Entity.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

PlaneCollider3D::PlaneCollider3D() : Collider3D("Plane Collider 3D", PLANE_COLLIDER_3D)
{
  RegisterDerivedType(PLANE_COLLIDER_3D);
}

void PlaneCollider3D::BuildGeometry()
{
	SetGeometrySilently(new physx::PxPlaneGeometry());
}

ZObject* PlaneCollider3D::CreateInstance(std::string name, ObjectType type)
{
	return new PlaneCollider3D();
}

ZObject* PlaneCollider3D::Copy(std::string name, ZObject* object)
{
	auto source = static_cast<PlaneCollider3D*>(object);
	auto copy = new PlaneCollider3D();

	return copy;
}

PlaneCollider3D::~PlaneCollider3D()
{
}
