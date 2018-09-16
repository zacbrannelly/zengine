#pragma once

#include "../Map/Objects/ZObject.h"
#include "../Scripting/IScriptable.h"
#include <include/v8.h>

class Entity;

class Component : public ZObject, public IScriptable
{
public:
	Component(std::string name, ObjectType type);
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(int viewId) = 0;

	virtual void SetOwner(Entity* owner);
	virtual Entity* GetOwner() const;

	static v8::Global<v8::FunctionTemplate> GetTemplate(v8::Isolate* isolate, v8::Local<v8::Object>& global);
private:
	Entity* _owner;
};

