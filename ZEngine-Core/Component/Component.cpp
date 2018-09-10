#include "Component.h"

using namespace std;

Component::Component(string name, ObjectType type) : ZObject(name, type), _owner(nullptr)
{
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
