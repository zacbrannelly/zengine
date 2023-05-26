#pragma once

#include "ZObject.h"
#include <vector>

class Component;
class Transform;

class Entity : public ZObject
{
public:
	Entity(std::string name); 
	~Entity();

	void AddComponent(Component* comp, bool shouldInit = false);
	void RemoveComponent(Component* comp);
	Component* GetComponent(ObjectType type) const;
	Component* GetComponentByName(std::string name) const;
	std::vector<Component*> GetComponents(ObjectType type) const;

	void InitComponents();
	void InitScripts();

	const std::vector<Component*>& GetAllComponents() const;
	Transform* GetTransform() const;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

#ifndef SWIGCSHARP
	static v8::Global<v8::FunctionTemplate> GetTemplate(v8::Isolate* isolate);
#endif

private:
	std::vector<Component*> _components;
	Transform* _transform;
};

