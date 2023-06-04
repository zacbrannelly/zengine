%module ZEngine

%{
  #include <Physics/Time.h>
%}

%ignore Time::Init;
%ignore Time::Tick;
%ignore Time::Shutdown;

%template(TimeSingleton) Singleton<Time>;

%include "Time.h"
