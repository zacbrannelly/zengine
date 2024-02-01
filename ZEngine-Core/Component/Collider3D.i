%module ZEngine

%{
#include <Component/Collider3D.h>
%}

%include "std_string.i"

%ignore Collider3D::BuildGeometry;
%ignore Collider3D::GetGeometry;

%include "Collider3D.h"
