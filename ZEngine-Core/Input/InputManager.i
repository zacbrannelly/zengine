%module ZEngine

%{
  #include <Input/InputManager.h>
%}

%include "std_string.i"
%include "std_vector.i"

%ignore InputManager::Init;
%ignore InputManager::Reset;
%ignore InputManager::Shutdown;
%ignore InputManager::RegisterKeyCallback;
%ignore InputManager::RegisterMousePosCallback;
%ignore InputManager::RegisterMouseButtonCallback;
%ignore InputManager::RemoveKeyCallback;
%ignore InputManager::RemoveMousePosCallback;
%ignore InputManager::RemoveMouseButtonCallback;
%ignore Display;

%template(InputManagerSingleton) Singleton<InputManager>;

%include "InputManager.h"
