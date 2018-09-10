#pragma once

#include "../Map/Objects/ZObject.h"

class Entity;

class Component : public ZObject
{
public:
	Component(std::string name, ObjectType type);
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void SetOwner(Entity* owner);
	virtual Entity* GetOwner() const;

private:
	Entity* _owner;
};

