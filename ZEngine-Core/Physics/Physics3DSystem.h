#pragma once

#include "../Misc/Singleton.h"
#include "../Utilities/JsonHelpers.h"
#include <string>
#include <stack>
#include <glm/glm.hpp>

namespace physx
{
  class PxDefaultAllocator;
  class PxDefaultErrorCallback;
  class PxFoundation;
  class PxPhysics;
  class PxDefaultCpuDispatcher;
  class PxScene;
  class PxMaterial;
  class PxCooking;
}

struct PhysicsSceneDescription
{
public:
  glm::vec3 gravity;

#ifndef SWIG
  JSON_SCHEMA_BEGIN(PhysicsSceneDescription)
    JSON_MAP_TO_MEMBER_OPTIONAL(gravity, gravity)
  JSON_SCHEMA_END()
#endif
};
typedef PhysicsSceneDescription PhysicsSceneDescription;

class Ray;
class Collider3D;

struct RaycastResult
{
  bool hit;
  float distance;
  glm::vec3 point;
  glm::vec3 normal;
  Collider3D* collider;
};
typedef RaycastResult RaycastResult;

class Physics3DSystem : public Singleton<Physics3DSystem>
{
public:
  bool Init();
  void Update();
  void Shutdown();

  physx::PxPhysics* GetPhysics() const { return _physics; }
  physx::PxCooking* GetCooking() const { return _cooking; }
  physx::PxScene* GetScene() const { return _sceneStack.top(); }
  physx::PxMaterial* GetMaterial() const { return _material; }

  bool Raycast(const Ray& ray, float maxDistance, RaycastResult& result);

  void PushScene();
  void PushScene(PhysicsSceneDescription description);
  void PopScene();

private:
  physx::PxDefaultAllocator* _allocator;
  physx::PxDefaultErrorCallback* _errorCallback;
  physx::PxFoundation* _foundation;
  physx::PxPhysics* _physics;
  physx::PxCooking* _cooking;
  physx::PxDefaultCpuDispatcher* _dispatcher;
  physx::PxMaterial* _material;
  std::stack<physx::PxScene*> _sceneStack;
};
