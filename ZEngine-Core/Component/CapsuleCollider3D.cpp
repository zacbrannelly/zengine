#include "CapsuleCollider3D.h"
#include "RigidBody3D.h"
#include "../Map/Objects/Entity.h"
#include "Transform.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

CapsuleCollider3D::CapsuleCollider3D() : DynamicCollider3D("Capsule Collider 3D", ObjectType::CAPSULE_COLLIDER_3D), _radius(1), _height(1)
{
	RegisterDerivedType(CAPSULE_COLLIDER_3D);
}

void CapsuleCollider3D::BuildGeometry()
{
	auto owner = GetOwner();
	if (owner == nullptr) return;

	auto geometry = new physx::PxCapsuleGeometry(GetWorldRadius(), GetWorldHeight() / 2);
	SetGeometrySilently(geometry);
}

void CapsuleCollider3D::SetSize(float radius, float height)
{
	_radius = radius;
	_height = height;
	OnGeometryChanged();
}

float CapsuleCollider3D::GetRadius() const
{
	return _radius;
}

float CapsuleCollider3D::GetHeight() const
{
	return _height;
}

float CapsuleCollider3D::GetWorldRadius() const
{
	auto owner = GetOwner();
	if (owner == nullptr) return _radius;

	auto actualRadius = _radius;
	auto transform = owner->GetTransform();
	auto scale = transform->GetWorldScale();

	return _radius * std::max(scale.y, scale.z);
}

float CapsuleCollider3D::GetWorldHeight() const
{
	auto owner = GetOwner();
	if (owner == nullptr) return _height;

	auto transform = owner->GetTransform();
	auto scale = transform->GetWorldScale();

	return std::max(_height * scale.x - 2 * GetWorldRadius(), 0.0f);
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
