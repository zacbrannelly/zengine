#include "Collider3D.h"
#include "RigidBody3D.h"
#include "Transform.h"
#include "../Map/Objects/Entity.h"
#include "../Physics/Physics3DSystem.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

using namespace ZEngine;

Collider3D::Collider3D(std::string name, ObjectType objectType) : Component(name, objectType), _geometry(nullptr), _staticBody(nullptr), _isTrigger(false)
{
  RegisterDerivedType(COLLIDER_3D);
}

void Collider3D::Init()
{
  auto isDynamicCollider = IsDerivedType(DYNAMIC_COLLIDER_3D);
  auto hasDynamicRigidBody = GetDynamicRigidBody() != nullptr;

  // Rigid Body component will handle injecting the collider into the physics system.
  if (isDynamicCollider && hasDynamicRigidBody) return;

  // Notify the physics system that a static collider has been added.
  OnGeometryChanged();
}

void Collider3D::SetIsTrigger(bool isTrigger)
{
  _isTrigger = isTrigger;
}

bool Collider3D::IsTrigger() const
{
  return _isTrigger;
}

void Collider3D::ApplyShapeFlags(physx::PxShape* shape)
{
  if (shape == nullptr) return;
  if (_isTrigger) 
  {
    shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
    shape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
  }
}

void Collider3D::SetGeometry(physx::PxGeometry* geometry, bool notify)
{
  if (GetGeometry() != nullptr) {
    auto geometry = GetGeometry();
    delete geometry;
  }
  _geometry = geometry;

  // Optionally notify the physics system that the collider has been modified.
  if (notify) OnGeometryChanged();
}

void Collider3D::SetGeometryAndNotify(physx::PxGeometry* geometry)
{
  SetGeometry(geometry, true);
}

void Collider3D::SetGeometrySilently(physx::PxGeometry* geometry)
{
  SetGeometry(geometry, false);
}

void Collider3D::OnGeometryChanged()
{
  // Try get the parent object.
  auto parent = GetOwner();
  if (parent == nullptr) return;

  // Try find a dynamic rigid body on the parent object.
  auto isDynamicCollider = IsDerivedType(DYNAMIC_COLLIDER_3D);
  auto dynamicRigidBody = parent->GetComponentByType<RigidBody3D>();
  if (dynamicRigidBody != nullptr && isDynamicCollider) 
  {
    // Notify the rigid body that the collider has been modified.
    dynamicRigidBody->OnColliderModified();
  }
  else
  {
    // If the parent object doesn't have a rigid body or the collider is static, create a static body.
    auto physics = Physics3DSystem::GetInstance();

    // Release any previously created bodies from the scene.
    if (_staticBody != nullptr) {
      physics->GetScene()->removeActor(*_staticBody);
      PX_RELEASE(_staticBody);
    }

    // Create PxTransform from existing transform.
    auto transform = parent->GetTransform();
    auto position = transform->GetPosition();
    auto quat = transform->GetRotationQuaternion();
    physx::PxTransform physxTransform(
      physx::PxVec3(position.x, position.y, position.z),
      physx::PxQuat(quat.x, quat.y, quat.z, quat.w)
    );
    _staticBody = physics->GetPhysics()->createRigidStatic(physxTransform);

    // Construct the geometry.
    BuildGeometry();
    if (_geometry == nullptr) return;

    // Create shape and attach to actor.
    auto shape = physics->GetPhysics()->createShape(*_geometry, *physics->GetMaterial(), true);
    shape->userData = this;

    // Apply shape flags - such as trigger settings.
    ApplyShapeFlags(shape);

    _staticBody->attachShape(*shape);
    shape->release();

    // Add to scene.
    physics->GetScene()->addActor(*_staticBody);
  }
}

RigidBody3D* Collider3D::GetDynamicRigidBody() const
{
  // Try get the parent object.
  auto parent = GetOwner();
  if (parent == nullptr) return nullptr;

  // Try find a dynamic rigid body on the parent object.
  return parent->GetComponentByType<RigidBody3D>();
}

physx::PxGeometry* Collider3D::GetGeometry() const
{
  return _geometry;
}

void Collider3D::OnDestroy()
{
  // Notify the rigid body that the collider has been removed.
  auto rigidBody = GetDynamicRigidBody();
  auto isDynamicCollider = IsDerivedType(DYNAMIC_COLLIDER_3D);
  if (rigidBody != nullptr && isDynamicCollider)
  {
    rigidBody->OnColliderModified();
  }

  // Release any previously created static bodies from the scene.
  if (_staticBody != nullptr)
  {
    auto physics = Physics3DSystem::GetInstance();
    physics->GetScene()->removeActor(*_staticBody);
    PX_RELEASE(_staticBody);
  }
}

Collider3D::~Collider3D()
{
  if (_geometry != nullptr)
  {
    delete _geometry;
    _geometry = nullptr;
  }
}
