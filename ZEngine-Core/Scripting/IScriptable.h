#pragma once

#include "../Map/Objects/ObjectType.h"

#ifndef SWIGCSHARP
#include <include/v8.h>
#endif

class IScriptable
{
public:
	IScriptable(ObjectType type);
	virtual ~IScriptable();

#ifndef SWIGCSHARP
	v8::Local<v8::Object> GetScriptObject() const;

protected:
	void SetScriptObject(v8::Local<v8::Object> scriptObj);

private:
	v8::Persistent<v8::Object> _scriptInstance;
#endif
};

