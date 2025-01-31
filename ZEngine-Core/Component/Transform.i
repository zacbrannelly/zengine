%module ZEngine

%{
#include <Component/Transform.h>
%}

%include "std_string.i"
%include "std_vector.i"

%template(TransformList) std::vector<ZEngine::Transform*>;

%include "Transform.h"
