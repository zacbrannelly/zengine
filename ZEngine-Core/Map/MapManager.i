%module ZEngine

%{
  #include <Map/MapManager.h>
%}

%include "std_string.i"
%include "std_vector.i"

%template(MapManagerSingleton) Singleton<MapManager>;

%include "MapManager.h"
