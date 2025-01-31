%module ZEngine

%{
#include <Physics/Physics3DSystem.h>
%}

%include "std_string.i"

%ignore ZEngine::Physics3DSystem::Init;
%ignore ZEngine::Physics3DSystem::Update;
%ignore ZEngine::Physics3DSystem::Shutdown;
%ignore ZEngine::Physics3DSystem::GetPhysics;
%ignore ZEngine::Physics3DSystem::GetScene;
%ignore ZEngine::Physics3DSystem::GetCooking;
%ignore ZEngine::Physics3DSystem::GetMaterial;
%ignore ZEngine::Physics3DSystem::PushScene;
%ignore ZEngine::Physics3DSystem::PopScene;

%ignore physx::PxDefaultAllocator;
%ignore physx::PxDefaultErrorCallback;
%ignore physx::PxFoundation;
%ignore physx::PxPhysics;
%ignore physx::PxDefaultCpuDispatcher;
%ignore physx::PxScene;
%ignore physx::PxMaterial;
%ignore physx::PxCooking;

%template(Physics3DSystemSingleton) ZEngine::Singleton<ZEngine::Physics3DSystem>;

%include "Physics3DSystem.h"
