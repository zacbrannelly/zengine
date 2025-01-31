#include "Component.h"
#include "../Map/Objects/Entity.h"

using namespace std;
using namespace ZEngine;

Component::Component(string name, ObjectType type) : ZObject(name, type), _owner(nullptr)
{
	RegisterDerivedType(COMPONENT);
}

void Component::SetOwner(Entity* owner)
{
	_owner = owner;
}

Entity* Component::GetOwner() const
{
	return _owner;
}

Component::~Component()
{
}
