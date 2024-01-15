#include "SphereCollider3D.h"
#include "RigidBody3D.h"
#include "../Map/Objects/Entity.h"
#include "Transform.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

SphereCollider3D::SphereCollider3D() : DynamicCollider3D("Sphere Collider 3D", ObjectType::SPHERE_COLLIDER_3D), _radius(1.0f)
{
	RegisterDerivedType(SPHERE_COLLIDER_3D);
}

void SphereCollider3D::SetRadius(float radius)
{
	_radius = radius;
	OnGeometryChanged();
}

void SphereCollider3D::BuildGeometry()
{
	auto owner = GetOwner();
	if (owner == nullptr) return;

	auto geometry = new physx::PxSphereGeometry(GetWorldRadius());
	SetGeometrySilently(geometry);
}

float SphereCollider3D::GetRadius() const
{
	return _radius;
}

float SphereCollider3D::GetWorldRadius() const
{
	auto owner = GetOwner();
	if (owner == nullptr) return _radius;

	auto actualRadius = _radius;
	auto transform = owner->GetTransform();
	auto scale = transform->GetWorldScale();

	auto maxScale = std::max({ scale.x, scale.y, scale.z });
	actualRadius *= maxScale;

	return actualRadius;
}

ZObject* SphereCollider3D::CreateInstance(std::string name, ObjectType type)
{
	return new SphereCollider3D();
}

ZObject* SphereCollider3D::Copy(std::string name, ZObject* object)
{
	auto source = static_cast<SphereCollider3D*>(object);
	auto copy = new SphereCollider3D();

	copy->SetRadius(source->GetRadius());

	return copy;
}

SphereCollider3D::~SphereCollider3D()
{
}
