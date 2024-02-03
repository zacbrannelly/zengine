#pragma once

#include "Component.h"

// TODO: Don't use library specific types in the engine
namespace physx 
{
  class PxGeometry;
  class PxRigidStatic;
  class PxShape;
}

class RigidBody3D;

class Collider3D : public Component
{
public:
  Collider3D(std::string name, ObjectType objectType);
  virtual ~Collider3D();

  virtual void Init() override;
  virtual void Update() override {}
  virtual void Render(int viewId) override {}
  virtual void OnDestroy() override;

  virtual void BuildGeometry() = 0;
  physx::PxGeometry* GetGeometry() const;

  void SetIsTrigger(bool isTrigger);
  bool IsTrigger() const;

  void ApplyShapeFlags(physx::PxShape* shape);

  static ObjectType GetStaticType() 
  {
    return COLLIDER_3D;
  }

protected:
  void SetGeometry(physx::PxGeometry* geometry, bool notify);
  void SetGeometryAndNotify(physx::PxGeometry* geometry);
  void SetGeometrySilently(physx::PxGeometry* geometry);
  void OnGeometryChanged();
  RigidBody3D* GetDynamicRigidBody() const;

private:
  physx::PxGeometry* _geometry;
  physx::PxRigidStatic* _staticBody;
  bool _isTrigger;
};
