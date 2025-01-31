%module ZEngine

%{
  #include <Map/MapManager.h>
%}

%include "std_string.i"
%include "std_vector.i"

%template(MapManagerSingleton) ZEngine::Singleton<ZEngine::MapManager>;

%include "MapManager.h"
