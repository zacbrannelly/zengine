%module ZEngine

%{
#include <Component/Camera.h>
%}

%include "std_string.i"

%ignore ZEngine::Camera::GetRenderTexture;

%include "Camera.h"
