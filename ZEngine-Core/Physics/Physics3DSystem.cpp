#define NDEBUG
#include <PxPhysicsAPI.h>

#include "Physics3DSystem.h"
#include <iostream>
#include <stdexcept>

using namespace physx;

bool Physics3DSystem::Init()
{
  _allocator = new PxDefaultAllocator();
  _errorCallback = new PxDefaultErrorCallback();

  // Setup foundation.
  _foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorCallback);
  if (!_foundation)
  {
    std::cerr << "PxCreateFoundation failed!" << std::endl;
    return false;
  }

  std::cout << "PhysX version: " << PX_PHYSICS_VERSION_MAJOR << "." << PX_PHYSICS_VERSION_MINOR << std::endl;

  // Setup physics.
  auto scale = PxTolerancesScale();
  _physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, scale);
  if (!_physics)
  {
    std::cerr << "PxCreatePhysics failed!" << std::endl;
    return false;
  }

  _cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, PxCookingParams(scale));
  if (!_cooking)
  {
    std::cerr << "PxCreateCooking failed!" << std::endl;
    return false;
  }

  // Setup CPU dispatcher.
  _dispatcher = PxDefaultCpuDispatcherCreate(1);

  // Create basic material.
  // TODO: Create Asset type for physics materials.
  _material = _physics->createMaterial(0.5f, 0.5f, 0.6f);

  // Create base scene.
  PushScene();

  return true;
}

void Physics3DSystem::PushScene()
{
  PhysicsSceneDescription sceneDesc;
  sceneDesc.gravity = glm::vec3(0.0f, -9.81f, 0.0f);
  PushScene(sceneDesc);
}

void Physics3DSystem::PushScene(PhysicsSceneDescription description)
{
  // Setup basic scene with gravity.
  PxSceneDesc sceneDesc(_physics->getTolerancesScale());
  sceneDesc.gravity = PxVec3(description.gravity.x, description.gravity.y, description.gravity.z);
  sceneDesc.cpuDispatcher = _dispatcher;
  sceneDesc.filterShader = PxDefaultSimulationFilterShader;

  auto scene = _physics->createScene(sceneDesc);
  _sceneStack.push(scene);
}

void Physics3DSystem::PopScene()
{
  if (_sceneStack.size() == 1) {
    throw std::runtime_error("Cannot pop scene stack with only one element");
  }

  auto poppedScene = _sceneStack.top();
  poppedScene->release();
  _sceneStack.pop();
}

void Physics3DSystem::Update()
{
  auto currentScene = _sceneStack.top();
  currentScene->simulate(1.0f / 60.0f);
  currentScene->fetchResults(true);
}

void Physics3DSystem::Shutdown()
{
  while (!_sceneStack.empty()) {
    auto scene = _sceneStack.top();
    scene->release();
    _sceneStack.pop();
  }
  PX_RELEASE(_dispatcher);
  PX_RELEASE(_cooking);
  PX_RELEASE(_physics);
  PX_RELEASE(_foundation);

  delete _errorCallback;
  delete _allocator;
}
