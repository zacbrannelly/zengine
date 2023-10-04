#include "CapsuleCollider3D.h"
#include "RigidBody3D.h"
#include "../Map/Objects/Entity.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

CapsuleCollider3D::CapsuleCollider3D() : Collider3D("Capsule Collider 3D", ObjectType::CAPSULE_COLLIDER_3D), _radius(1), _height(1)
{
	RegisterDerivedType(CAPSULE_COLLIDER_3D);
	SetSize(_radius, _height);
}

void CapsuleCollider3D::SetSize(float radius, float height)
{
	_radius = radius;
	_height = height;

	auto geometry = new physx::PxCapsuleGeometry(radius, height / 2);
	SetGeometry(geometry);
}

float CapsuleCollider3D::GetRadius() const
{
	return _radius;
}

float CapsuleCollider3D::GetHeight() const
{
	return _height;
}

ZObject* CapsuleCollider3D::CreateInstance(std::string name, ObjectType type)
{
	return new CapsuleCollider3D();
}

ZObject* CapsuleCollider3D::Copy(std::string name, ZObject* object)
{
	auto source = static_cast<CapsuleCollider3D*>(object);
	auto copy = new CapsuleCollider3D();

	copy->SetSize(source->GetRadius(), source->GetHeight());

	return copy;
}

CapsuleCollider3D::~CapsuleCollider3D()
{
}
