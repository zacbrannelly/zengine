%module ZEngine

%{
#include <Component/Transform.h>
%}

%include "std_string.i"
%include "std_vector.i"

%template(TransformList) std::vector<Transform*>;

%include "Transform.h"
