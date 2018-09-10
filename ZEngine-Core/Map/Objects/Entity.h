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

	void AddComponent(Component* comp);
	void RemoveComponent(Component* comp);
	Component* GetComponent(ObjectType type) const;
	std::vector<Component*> GetComponents(ObjectType type) const;

	const std::vector<Component*>& GetAllComponents() const;
	Transform* GetTransform() const;

	static ZObject* CreateInstance(std::string name, ObjectType type);

private:
	std::vector<Component*> _components;
	Transform* _transform;
};

