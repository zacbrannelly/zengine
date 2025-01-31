%module ZEngine

%{
  #include <Physics/Time.h>
%}

%ignore ZEngine::Time::Init;
%ignore ZEngine::Time::Tick;
%ignore ZEngine::Time::Shutdown;

%template(TimeSingleton) ZEngine::Singleton<ZEngine::Time>;

%include "Time.h"
