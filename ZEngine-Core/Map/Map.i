%module ZEngine

%{
  #include <Map/Map.h>
%}

%include "std_string.i"
%include "std_vector.i"

%template(EntityList) std::vector<Entity*>;
%template(CameraList) std::vector<Camera*>;

%ignore Map::Init;
%ignore Map::Update;
%ignore Map::Render;
%ignore Map::RenderWorld;
%ignore Map::RegisterCameras;
%ignore Map::CreateInstance;
%ignore Map::Copy;

%include "Map.h"
