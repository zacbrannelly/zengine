#include "PlaneCollider3D.h"
#include "RigidBody3D.h"
#include "../Map/Objects/Entity.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

using namespace ZEngine;

PlaneCollider3D::PlaneCollider3D() : Collider3D("Plane Collider 3D", PLANE_COLLIDER_3D)
{
  RegisterDerivedType(PLANE_COLLIDER_3D);
}

void PlaneCollider3D::SetDirection(glm::vec3 direction)
{
	_direction = direction;
}

const glm::vec3& PlaneCollider3D::GetDirection() const
{
	return _direction;
}

void PlaneCollider3D::SetDistance(float distance)
{
	_distance = distance;
}

float PlaneCollider3D::GetDistance() const
{
	return _distance;
}

void PlaneCollider3D::BuildGeometry()
{
	// TODO: Use the `direction` and `distance` to create the plane geometry
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
	copy->SetIsTrigger(source->IsTrigger());

	return copy;
}

PlaneCollider3D::~PlaneCollider3D()
{
}
