%module ZEngine

%{
#include <Map/Objects/Entity.h>
%}

%include "std_string.i"
%include "std_vector.i"

// Ignore templated functions.
// TODO: Figure out how to expose these to C#.
%ignore ZEngine::Entity::HasComponentType;
%ignore ZEngine::Entity::GetComponentByType;
%ignore ZEngine::Entity::GetComponentsByType;

%template(ComponentList) std::vector<ZEngine::Component*>;

// Create a constructor that assumes it doesn't own the pointer.
%typemap(cscode) ZEngine::Entity %{
  internal Entity(global::System.IntPtr cPtr) : this(cPtr, false) {}
%}

%include "Entity.h"
