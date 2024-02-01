%module ZEngine

%{
#include <Component/Camera.h>
%}

%include "std_string.i"

%ignore Camera::GetRenderTexture;

%include "Camera.h"
