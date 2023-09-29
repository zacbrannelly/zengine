#pragma once

#include "Component.h"
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

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

  static ObjectType GetStaticType()
  {
    return RIGID_BODY_3D;
  }

private:
  physx::PxRigidBody* _rigidBody;
  float _mass;
  float _linearDamping;
  float _angularDamping;
  bool _useGravity;
  bool _isKinematic;
  glm::vec3 _kinematicTarget;
};
