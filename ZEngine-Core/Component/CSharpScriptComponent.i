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

// Expose a public constructor to C# that takes a pointer to an existing C++ object.
%typemap(cscode) CSharpScriptComponent %{
  public CSharpScriptComponent(global::System.IntPtr cPtr) : base(cPtr) {}
%}

%include "CSharpScriptComponent.h"
