#pragma once

#include "ZObject.h"
#include <vector>

class Component;

class Entity : public ZObject
{
public:
	Entity(std::string name); 
	~Entity();

	void AddComponent(Component* comp);

	Component* GetComponent(ObjectType type) const;
	std::vector<Component*> GetComponents(ObjectType type) const;

	static ZObject* CreateInstance(std::string name, ObjectType type);

private:
	std::vector<Component*> _components;
};

