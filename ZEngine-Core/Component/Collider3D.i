%module ZEngine

%{
#include <Component/Collider3D.h>
%}

%include "std_string.i"

%ignore ZEngine::Collider3D::BuildGeometry;
%ignore ZEngine::Collider3D::GetGeometry;

%include "Collider3D.h"
