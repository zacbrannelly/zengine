#include "RigidBody3D.h"
#include "../Physics/Physics3DSystem.h"
#include "../Map/Objects/Entity.h"
#include "Transform.h"
#include "DynamicCollider3D.h"

#define NDEBUG
#include <PxPhysicsAPI.h>

#include <string>
#include <iostream>

using namespace std;
using namespace physx;

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
  auto transform = GetOwner()->GetTransform();

  // Release any previously created bodies from the scene.
  if (_rigidBody != nullptr) {
    physics->GetScene()->removeActor(*_rigidBody);
    PX_RELEASE(_rigidBody);
  }

  // Create PxTransform from existing transform.
  auto position = transform->GetPosition();
  PxTransform physxTransform(PxVec3(position.x, position.y, position.z));

  // Fetch collider from the parent object.
  auto entity = GetOwner();
  auto collider = entity->GetComponentByType<DynamicCollider3D>();

  if (collider == nullptr) {
    cout << "RigidBody3D::Init() - No collider found on parent object." << endl;
    return;
  }
  auto geometry = collider->GetGeometry();

  auto sdk = physics->GetPhysics();
  auto scene = physics->GetScene();

  // Create dynamic actor with a single shape.
  PxRigidDynamic* dynamic = PxCreateDynamic(*physics->GetPhysics(), physxTransform, *geometry, *physics->GetMaterial(), 10.0f);
  
  // Create shape and attach to actor.
  auto shape = sdk->createShape(*geometry, *physics->GetMaterial(), true);
  dynamic->attachShape(*shape);
  shape->release();

  // Calculate mass and inertia based on the shape and density.
  // TODO: Make this optional & configurable.
  PxRigidBodyExt::updateMassAndInertia(*dynamic, 10.0f);

  // Add actor to the scene.
	scene->addActor(*dynamic);

  // Store the rigid body.
  _rigidBody = dynamic;

  // Set mass, linear damping, angular damping, and gravity.
  if (_mass > 0) SetMass(_mass);
  SetLinearDamping(_linearDamping);
  SetAngularDamping(_angularDamping);
  SetUseGravity(_useGravity);
  SetIsKinematic(_isKinematic);
  SetKinematicTarget(_kinematicTarget);
}

void RigidBody3D::OnColliderModified()
{
  if (_rigidBody == nullptr) return;

  PxShape* shapes[1];
  _rigidBody->getShapes(shapes, 1);

  auto physics = Physics3DSystem::GetInstance();
  auto sdk = physics->GetPhysics();

  auto collider = GetOwner()->GetComponentByType<DynamicCollider3D>();
  if (collider != nullptr)
  {
    // Detach the previous shape from the actor.
    _rigidBody->detachShape(*shapes[0]);
    
    // Create new shape and attach to actor.
    auto geometry = collider->GetGeometry();
    auto shape = sdk->createShape(*geometry, *physics->GetMaterial(), true);
    _rigidBody->attachShape(*shape);

    // Calculate mass and inertia based on the shape and density.
    // TODO: Make this optional & configurable.
    PxRigidBodyExt::updateMassAndInertia(*_rigidBody, 10.0f);
  }
  else
  {
    cout << "RigidBody3D::OnColliderModified() - No collider found on parent object." << endl;

    // Remove the rigid body from the scene.
    OnDestroy();
  }
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

void RigidBody3D::Update()
{
  auto transform = GetOwner()->GetTransform();
  auto pose = _rigidBody->getGlobalPose();
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
