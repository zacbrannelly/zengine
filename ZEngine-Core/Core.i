%module ZEngine

// Converts void* to System.IntPtr in C#.
%apply void* VOID_INT_PTR { void* }

// Bindings for glm.
%include "Scripting/CSharp/glm/glm.i"

// ZEngine Enum bindings.
%include "Map/Objects/ObjectType.h"
%include "Input/InputTypes.h"

// ZEngine Class bindings.
%include "Misc/Singleton.i"
%include "Map/Objects/ZObject.i"
%include "Misc/Factory.i"
%include "Map/Objects/Entity.i"
%include "Component/Component.i"
%include "Component/Transform.i"
%include "Component/AudioSource.i"
%include "Input/InputManager.i"
%include "Component/CSharpScriptComponent.i"
%include "Physics/Time.i"
%include "Map/Map.i"
%include "Map/MapManager.i"
