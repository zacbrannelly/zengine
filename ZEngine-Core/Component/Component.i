%module ZEngine

%{
#include <Component/Component.h>
%}

%include "std_string.i"

// Expose a public constructor to C# that takes a pointer to an existing C++ object.
%typemap(cscode) Component %{
  public Component(global::System.IntPtr cPtr) : this(cPtr, false) {}
%}

%include "Component.h"
