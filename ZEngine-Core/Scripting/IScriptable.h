#pragma once

enum ObjectType;

#include <include/v8.h>

class IScriptable
{
public:
	IScriptable(ObjectType type);
	virtual ~IScriptable();

	v8::Local<v8::Object> GetScriptObject() const;

protected:
	void SetScriptObject(v8::Local<v8::Object> scriptObj);

private:
	v8::Persistent<v8::Object> _scriptInstance;
};

