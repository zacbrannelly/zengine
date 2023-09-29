#pragma once

#include "../Misc/Singleton.h"
#include <string>
#include <stack>

namespace physx
{
  class PxDefaultAllocator;
  class PxDefaultErrorCallback;
  class PxFoundation;
  class PxPhysics;
  class PxDefaultCpuDispatcher;
  class PxScene;
  class PxMaterial;
}

class Physics3DSystem : public Singleton<Physics3DSystem>
{
public:
  bool Init();
  void Update();
  void Shutdown();

  physx::PxPhysics* GetPhysics() const { return _physics; }
  physx::PxScene* GetScene() const { return _sceneStack.top(); }
  physx::PxMaterial* GetMaterial() const { return _material; }

  void PushScene();
  void PopScene();

private:
  physx::PxDefaultAllocator* _allocator;
  physx::PxDefaultErrorCallback* _errorCallback;
  physx::PxFoundation* _foundation;
  physx::PxPhysics* _physics;
  physx::PxDefaultCpuDispatcher* _dispatcher;
  physx::PxMaterial* _material;
  std::stack<physx::PxScene*> _sceneStack;
};
