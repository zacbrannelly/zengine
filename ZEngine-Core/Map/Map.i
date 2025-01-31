%module ZEngine

%{
  #include <Map/Map.h>
%}

%include "std_string.i"
%include "std_vector.i"

%template(EntityList) std::vector<ZEngine::Entity*>;
%template(CameraList) std::vector<ZEngine::Camera*>;

%ignore ZEngine::Map::Init;
%ignore ZEngine::Map::Update;
%ignore ZEngine::Map::Render;
%ignore ZEngine::Map::RenderWorld;
%ignore ZEngine::Map::RegisterCameras;
%ignore ZEngine::Map::CreateInstance;
%ignore ZEngine::Map::Copy;

%include "Map.h"
