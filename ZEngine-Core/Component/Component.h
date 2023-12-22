#pragma once

#include "../Map/Objects/ZObject.h"

class Entity;

class Component : public ZObject
{
public:
	Component(std::string name, ObjectType type);
	virtual ~Component();

    // Required hooks
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(int viewId) = 0;
    
    // Optional hooks
	virtual void OnDestroy() {}
    virtual void RenderGizmos(int viewId) {}

	virtual void SetOwner(Entity* owner);
	virtual Entity* GetOwner() const;

	static ObjectType GetStaticType()
	{
		return COMPONENT;
	}
private:
	Entity* _owner;
};

