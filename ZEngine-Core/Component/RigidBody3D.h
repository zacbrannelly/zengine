#pragma once

#include "Component.h"
#include "../Utilities/JsonHelpers.h"
#include <glm/glm.hpp>

// TODO: Don't use library specific types in the engine
namespace physx 
{
  class PxRigidBody;
}

class RigidBody3D : public Component
{
public:
	RigidBody3D();
	~RigidBody3D();

	void Init() override;
	void Update() override;
	void Render(int viewId) override;
  void OnDestroy() override;

  void OnColliderModified();

  void SetMass(float mass);
  float GetMass() const;

  void SetLinearDamping(float damping);
  float GetLinearDamping() const;

  void SetAngularDamping(float damping);
  float GetAngularDamping() const;

  void SetUseGravity(bool useGravity);
  bool GetUseGravity() const;

  void SetIsKinematic(bool isKinematic);
  bool GetIsKinematic() const;

  void SetKinematicTarget(const glm::vec3& target);
  glm::vec3 GetKinematicTarget() const;

  void ApplyForce(const glm::vec3& force);
  void ApplyForceAtPosition(const glm::vec3& force, const glm::vec3& position);
  void ApplyTorque(const glm::vec3& torque);

  void ApplyImpulse(const glm::vec3& impulse);
  void ApplyImpulseAtPosition(const glm::vec3& impulse, const glm::vec3& position);
  void ApplyTorqueImpulse(const glm::vec3& torqueImpulse);

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

  static ObjectType GetStaticType()
  {
    return RIGID_BODY_3D;
  }

#ifndef SWIG
  // Allow serialization / deserialization
  JSON_SCHEMA_BEGIN(RigidBody3D)
    JSON_TO_SETTER_OPTIONAL (mass,           SetMass,           float)
    JSON_TO_SETTER_OPTIONAL (linearDamping,  SetLinearDamping,  float)
    JSON_TO_SETTER_OPTIONAL (angularDamping, SetAngularDamping, float)
    JSON_TO_SETTER_OPTIONAL (useGravity,     SetUseGravity,     bool)
    JSON_TO_SETTER_OPTIONAL (isKinematic,    SetIsKinematic,    bool)
  JSON_SCHEMA_END()
#endif

private:
  void RemoveAllShapes();
  void RebuildActorShapes();

  physx::PxRigidBody* _rigidBody;
  float _mass;
  float _linearDamping;
  float _angularDamping;
  bool _useGravity;
  bool _isKinematic;
  glm::vec3 _kinematicTarget;
};
