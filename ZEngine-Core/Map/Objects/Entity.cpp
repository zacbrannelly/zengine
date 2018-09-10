#include "Entity.h"
#include "../../Component/Component.h"

using namespace std;

Entity::Entity(string name) : ZObject(name, ObjectType::ENTITY)
{
}

ZObject* Entity::CreateInstance(string name, ObjectType type)
{
	return new Entity(name);
}

void Entity::AddComponent(Component* component)
{
	if (component != nullptr)
	{
		_components.push_back(component);

		component->SetOwner(this);
		component->Init();
	}
}

Component* Entity::GetComponent(ObjectType type) const
{
	for (auto c : _components)
	{
		if (c->GetType() == type)
			return c;
	}

	return nullptr;
}

vector<Component*> Entity::GetComponents(ObjectType type) const
{
	vector<Component*> results;

	for (auto c : _components)
	{
		if (c->GetType() == type)
			results.push_back(c);
	}

	return results;
}


Entity::~Entity()
{
	for (auto component : _components)
		delete component;
}
