#pragma once

#include "../Map/Objects/ZObject.h"

#ifndef SWIGCSHARP
#include <include/v8.h>
#endif

class Entity;

class Component : public ZObject
{
public:
	Component(std::string name, ObjectType type);
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(int viewId) = 0;

	virtual void SetOwner(Entity* owner);
	virtual Entity* GetOwner() const;

#ifndef SWIGCSHARP
	static v8::Global<v8::FunctionTemplate> GetTemplate(v8::Isolate* isolate, v8::Local<v8::Object>& global);
#endif

private:
	Entity* _owner;
};

