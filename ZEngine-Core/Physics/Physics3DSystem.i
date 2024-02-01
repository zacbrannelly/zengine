%module ZEngine

%{
#include <Physics/Physics3DSystem.h>
%}

%include "std_string.i"

%ignore Physics3DSystem::Init;
%ignore Physics3DSystem::Update;
%ignore Physics3DSystem::Shutdown;
%ignore Physics3DSystem::GetPhysics;
%ignore Physics3DSystem::GetScene;
%ignore Physics3DSystem::GetCooking;
%ignore Physics3DSystem::GetMaterial;
%ignore Physics3DSystem::PushScene;
%ignore Physics3DSystem::PopScene;

%ignore physx::PxDefaultAllocator;
%ignore physx::PxDefaultErrorCallback;
%ignore physx::PxFoundation;
%ignore physx::PxPhysics;
%ignore physx::PxDefaultCpuDispatcher;
%ignore physx::PxScene;
%ignore physx::PxMaterial;
%ignore physx::PxCooking;

%template(Physics3DSystemSingleton) Singleton<Physics3DSystem>;

%include "Physics3DSystem.h"
