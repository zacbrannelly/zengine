#include "RigidBody3D.h"
#include "../Physics/Physics3DSystem.h"
#include "../Physics/PhysxUtils.h"
#include "../Map/Objects/Entity.h"
#include "Transform.h"
#include "DynamicCollider3D.h"

#define NDEBUG
#include <PxPhysicsAPI.h>
#include <extensions/PxRigidBodyExt.h>

#include <string>
#include <iostream>

using namespace std;
using namespace physx;
using namespace ZEngine;

RigidBody3D::RigidBody3D() : Component("Rigid Body 3D", ObjectType::RIGID_BODY_3D), _rigidBody(nullptr)
{
  RegisterDerivedType(RIGID_BODY_3D);
  _mass = -1.0f;
  _linearDamping = 0.0f;
  _angularDamping = 0.0f;
  _useGravity = true;
  _isKinematic = false;
  _kinematicTarget = glm::vec3(0.0f);
}

void RigidBody3D::Init()
{
  auto physics = Physics3DSystem::GetInstance();
  auto scene = physics->GetScene();

  // Release any previously created bodies from the scene.
  if (_rigidBody != nullptr) {
    scene->removeActor(*_rigidBody);
    PX_RELEASE(_rigidBody);
  }

  // Create PxTransform from existing transform.
  auto transform = GetOwner()->GetTransform();
  auto position = transform->GetPosition();
  PxTransform physxTransform(PxVec3(position.x, position.y, position.z));

  // Create dynamic actor.
  auto sdk = physics->GetPhysics();
  _rigidBody = sdk->createRigidDynamic(physxTransform);
  RebuildActorShapes();

  // Add actor to the scene.
	scene->addActor(*_rigidBody);

  // Set mass, linear damping, angular damping, and gravity.
  if (_mass > 0) SetMass(_mass);
  SetLinearDamping(_linearDamping);
  SetAngularDamping(_angularDamping);
  SetUseGravity(_useGravity);
  SetIsKinematic(_isKinematic);
  SetKinematicTarget(_kinematicTarget);
}

void RigidBody3D::RebuildActorShapes()
{
  if (_rigidBody == nullptr) return;

  auto physics = Physics3DSystem::GetInstance();
  auto sdk = physics->GetPhysics();
  auto scene = physics->GetScene();
  
  auto numShapes = _rigidBody->getNbShapes();
  if (numShapes > 0) RemoveAllShapes();

  auto entity = GetOwner();
  auto colliders = entity->GetComponentsByType<DynamicCollider3D>();

  // Ensure at least one collider is present, otherwise remove the rigid body.
  if (colliders.size() == 0) {
    cout << "RigidBody3D::Init() - No colliders found on parent object." << endl;
    OnDestroy();
    return;
  }

  // Create shape and attach to actor.
  for (auto collider : colliders) 
  {
    // Build the geometry.
    collider->BuildGeometry();
    auto geometry = collider->GetGeometry();
    if (geometry == nullptr)
    {
      cout << "RigidBody3D::Init() - Failed to create geometry for collider." << endl;
      continue;
    }

    // Create shape from geometry.
    auto shape = sdk->createShape(*geometry, *physics->GetMaterial(), true);
    shape->userData = collider;

    // Apply shape flags - such as trigger settings.
    collider->ApplyShapeFlags(shape);

    // Attach shape to actor.
    _rigidBody->attachShape(*shape);
    shape->release();
  }

  // Apply mass and inertia based on the shapes and density.
  // TODO: Make this optional & configurable.
  PxRigidBodyExt::updateMassAndInertia(*_rigidBody, 10.0f);
}

void RigidBody3D::RemoveAllShapes()
{
  if (_rigidBody == nullptr) return;

  auto numShapes = _rigidBody->getNbShapes();
  auto shapes = new PxShape*[numShapes];
  _rigidBody->getShapes(shapes, numShapes);

  for (auto i = 0; i < numShapes; ++i)
  {
    _rigidBody->detachShape(*shapes[i]);
  }
}

void RigidBody3D::OnColliderModified()
{
  RebuildActorShapes();
}

void RigidBody3D::SetMass(float mass)
{
  _mass = mass;
  if (_rigidBody != nullptr) _rigidBody->setMass(mass);
}

float RigidBody3D::GetMass() const
{
  return _rigidBody != nullptr 
    ? _rigidBody->getMass() 
    : _mass;
}

void RigidBody3D::SetLinearDamping(float damping)
{
  _linearDamping = damping;
  if (_rigidBody != nullptr) _rigidBody->setLinearDamping(damping);
}

float RigidBody3D::GetLinearDamping() const
{
  return _rigidBody != nullptr
    ? _rigidBody->getLinearDamping()
    : _linearDamping;
}

void RigidBody3D::SetAngularDamping(float damping)
{
  _angularDamping = damping;
  if (_rigidBody != nullptr) _rigidBody->setAngularDamping(damping);
}

float RigidBody3D::GetAngularDamping() const
{
  return _rigidBody != nullptr
    ? _rigidBody->getAngularDamping()
    : _angularDamping;
}

void RigidBody3D::SetUseGravity(bool useGravity)
{
  _useGravity = useGravity;
  if (_rigidBody != nullptr) _rigidBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !useGravity);
}

bool RigidBody3D::GetUseGravity() const
{
  return _rigidBody != nullptr
    ? !_rigidBody->getActorFlags().isSet(PxActorFlag::eDISABLE_GRAVITY)
    : _useGravity;
}

void RigidBody3D::SetIsKinematic(bool isKinematic)
{
  _isKinematic = isKinematic;
  if (_rigidBody != nullptr) _rigidBody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
}

bool RigidBody3D::GetIsKinematic() const
{
  return _rigidBody != nullptr
    ? _rigidBody->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC)
    : _isKinematic;
}

void RigidBody3D::SetKinematicTarget(const glm::vec3& target)
{
  _kinematicTarget = target;
  if (_rigidBody == nullptr) return;

  auto dynamicRigidBody = _rigidBody->is<physx::PxRigidDynamic>();
  if (dynamicRigidBody && GetIsKinematic()) 
  {
    dynamicRigidBody->setKinematicTarget(PxTransform(PxVec3(target.x, target.y, target.z)));
  }
}

glm::vec3 RigidBody3D::GetKinematicTarget() const
{
  if (_rigidBody == nullptr) return _kinematicTarget;

  auto dynamicRigidBody = _rigidBody->is<physx::PxRigidDynamic>();
  if (dynamicRigidBody)
  {
    PxTransform target;
    auto isSet = dynamicRigidBody->getKinematicTarget(target);
    if (isSet)
    {
      return glm::vec3(target.p.x, target.p.y, target.p.z);
    }
  }

  return glm::vec3(0.0f);
}

void RigidBody3D::ApplyForce(const glm::vec3& force)
{
  if (_rigidBody == nullptr) return;
  _rigidBody->addForce(ToPxVec3(force));
}

void RigidBody3D::ApplyForceAtPosition(const glm::vec3& force, const glm::vec3& position)
{
  if (_rigidBody == nullptr) return;
  physx::PxRigidBodyExt::addForceAtPos(*_rigidBody, ToPxVec3(force), ToPxVec3(position));
}

void RigidBody3D::ApplyTorque(const glm::vec3& torque)
{
  if (_rigidBody == nullptr) return;
  _rigidBody->addTorque(ToPxVec3(torque));
}

void RigidBody3D::ApplyImpulse(const glm::vec3& impulse)
{
  if (_rigidBody == nullptr) return;
  _rigidBody->addForce(ToPxVec3(impulse), PxForceMode::eIMPULSE);
}

void RigidBody3D::ApplyImpulseAtPosition(const glm::vec3& impulse, const glm::vec3& position)
{
  if (_rigidBody == nullptr) return;
  physx::PxRigidBodyExt::addForceAtPos(*_rigidBody, ToPxVec3(impulse), ToPxVec3(position), PxForceMode::eIMPULSE);
}

void RigidBody3D::ApplyTorqueImpulse(const glm::vec3& torqueImpulse)
{
  if (_rigidBody == nullptr) return;
  _rigidBody->addTorque(ToPxVec3(torqueImpulse), PxForceMode::eIMPULSE);
}

void RigidBody3D::Update()
{
  auto transform = GetOwner()->GetTransform();
  auto pose = _rigidBody->getGlobalPose();

  // TODO: Set position and rotation in world space, not local space.
  transform->SetPosition(glm::vec3(pose.p.x, pose.p.y, pose.p.z));
  transform->SetRotationQuaternion(glm::quat(pose.q.w, pose.q.x, pose.q.y, pose.q.z));
}

void RigidBody3D::Render(int viewId)
{
}

void RigidBody3D::OnDestroy()
{
  auto physics = Physics3DSystem::GetInstance();
  physics->GetScene()->removeActor(*_rigidBody);
  PX_RELEASE(_rigidBody);
}

RigidBody3D::~RigidBody3D()
{
}

ZObject* RigidBody3D::CreateInstance(string name, ObjectType type)
{
	return new RigidBody3D();
}

ZObject* RigidBody3D::Copy(string name, ZObject* object)
{
	if (object == nullptr || object->GetType() != RIGID_BODY_3D)
		return nullptr;

	auto source = static_cast<RigidBody3D*>(object);
	auto copy = new RigidBody3D();

  copy->SetMass(source->GetMass());
  copy->SetLinearDamping(source->GetLinearDamping());
  copy->SetAngularDamping(source->GetAngularDamping());
  copy->SetUseGravity(source->GetUseGravity());
  copy->SetIsKinematic(source->GetIsKinematic());
  copy->SetKinematicTarget(source->GetKinematicTarget());

	return copy;
}
