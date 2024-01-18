#pragma once

#include "../Misc/Singleton.h"
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
} typedef PhysicsSceneDescription;

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
