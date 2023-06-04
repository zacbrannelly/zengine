%module ZEngine

%{
#include <Component/Component.h>
%}

%include "std_string.i"

%ignore Component::Init;
%ignore Component::Update;
%ignore Component::Render;

%typemap(cscode) Component %{
  // Expose a public constructor to C# that takes a pointer to an existing C++ object.
  // TODO: Remove this if its not used.
  public Component(global::System.IntPtr cPtr) : this(cPtr, false) {}

  // Empty constructor to allow for creation without a pointer.
  internal Component() : base() {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
  }

  // Allow setting the pointer to an existing object.
  internal override void SetCPtr(global::System.IntPtr cPtr, bool cMemoryOwn) {
    base.SetCPtr(ZEnginePINVOKE.Component_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }
%}

%include "Component.h"
