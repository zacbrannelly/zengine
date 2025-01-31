%module ZEngine

%{
  #include <Input/InputManager.h>
%}

%include "std_string.i"
%include "std_vector.i"

%ignore ZEngine::InputManager::Init;
%ignore ZEngine::InputManager::Reset;
%ignore ZEngine::InputManager::Shutdown;
%ignore ZEngine::InputManager::RegisterKeyCallback;
%ignore ZEngine::InputManager::RegisterMousePosCallback;
%ignore ZEngine::InputManager::RegisterMouseButtonCallback;
%ignore ZEngine::InputManager::RemoveKeyCallback;
%ignore ZEngine::InputManager::RemoveMousePosCallback;
%ignore ZEngine::InputManager::RemoveMouseButtonCallback;
%ignore ZEngine::Display;

%template(InputManagerSingleton) ZEngine::Singleton<ZEngine::InputManager>;

%include "InputManager.h"
