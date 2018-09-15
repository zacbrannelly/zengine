#pragma once

#include "../Map/Objects/ZObject.h"
#include <include/v8.h>

class IScriptable
{
public:
	IScriptable(ObjectType type);
	virtual ~IScriptable();

	v8::Local<v8::Object> GetScriptObject() const;

private:
	v8::Persistent<v8::Object> _scriptInstance;
};

