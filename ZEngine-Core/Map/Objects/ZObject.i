%module ZEngine

%{
#include <Map/Objects/ZObject.h>
%}

%include "std_string.i"
%include "std_vector.i"

%ignore ZObject::CreateInstance;

%typemap(cscode) ZObject %{
  // Empty constructor to allow for creation without a pointer.
  internal ZObject() {
    swigCMemOwn = false;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
  }

  // Allow setting the pointer to an existing object.
  internal virtual void SetCPtr(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
    swigCMemOwn = cMemoryOwn;
  }
%}

%include "ZObject.h"
