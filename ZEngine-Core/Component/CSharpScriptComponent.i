%module ZEngine

%{
#include <Component/CSharpScriptComponent.h>
%}

%include "std_string.i"

%ignore CSharpScriptComponent::GetScript;
%ignore CSharpScriptComponent::SetScript;
%ignore CSharpScriptComponent::Init;
%ignore CSharpScriptComponent::Update;
%ignore CSharpScriptComponent::Render;

%typemap(cscode) CSharpScriptComponent %{
  // Expose a public constructor to C# that takes a pointer to an existing C++ object.
  // TODO: Removed if it is not used.
  public CSharpScriptComponent(global::System.IntPtr cPtr) : base(cPtr) {}

  // Public empty constructor, so user scripts can define their own constructors.
  public CSharpScriptComponent() : base() {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
  }

  // Allow setting the pointer to an existing object.
  internal override void SetCPtr(global::System.IntPtr cPtr, bool cMemoryOwn) {
    base.SetCPtr(ZEnginePINVOKE.CSharpScriptComponent_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  public T? GetComponent<T>() where T : Component
  {
    return Entity.GetComponent<T>();
  }

  public Entity Entity => GetOwner();

  public Transform Transform => Entity.GetTransform();
%}

%include "CSharpScriptComponent.h"
