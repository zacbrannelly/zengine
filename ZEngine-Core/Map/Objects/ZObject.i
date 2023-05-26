%module ZEngine

%{
#include <Map/Objects/ZObject.h>
%}

%include "std_string.i"
%include "std_vector.i"

%ignore ZObject::CreateInstance;

%include "ZObject.h"
