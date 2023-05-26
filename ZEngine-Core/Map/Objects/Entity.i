%module ZEngine

%{
#include <Map/Objects/Entity.h>
%}

%include "std_string.i"
%include "std_vector.i"

%template(ComponentList) std::vector<Component*>;

// Create a constructor that assumes it doesn't own the pointer.
%typemap(cscode) Entity %{
  internal Entity(global::System.IntPtr cPtr) : this(cPtr, false) {}
%}

%include "Entity.h"
