#include "Entity.h"
#include "../../Component/Component.h"
#include "../../Misc/Factory.h"
#include "../../Component/Transform.h"

using namespace std;

Entity::Entity(string name) : ZObject(name, ObjectType::ENTITY)
{
	// Add transform component by default
	_transform = Factory::CreateInstance<Transform>("Transform", ObjectType::TRANSFORM);
	AddComponent(_transform);
}

ZObject* Entity::CreateInstance(string name, ObjectType type)
{
	return new Entity(name);
}

void Entity::AddComponent(Component* component)
{
	// Allow valid components and ONLY one transform component 
	if (component != nullptr || (component != nullptr && component->GetType() == ObjectType::TRANSFORM && _transform == nullptr))
	{
		_components.push_back(component);

		component->SetOwner(this);
		component->Init();
	}
}

void Entity::RemoveComponent(Component* component)
{
	auto it = find(_components.begin(), _components.end(), component);

	if (it != _components.end())
	{
		_components.erase(it);

		// TODO: Send message to component that it has been removed
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

const std::vector<Component*>& Entity::GetAllComponents() const
{
	return _components;
}

Transform* Entity::GetTransform() const
{
	return _transform;
}

Entity::~Entity()
{
	for (auto component : _components)
		delete component;
}
