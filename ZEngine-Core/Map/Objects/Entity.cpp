#include "Entity.h"
#include "../../Component/Component.h"
#include "../../Misc/Factory.h"
#include "../../Component/Transform.h"

using namespace std;
using namespace ZEngine;

Entity::Entity(string name) : ZObject(name, ObjectType::ENTITY)
{	
	RegisterDerivedType(ENTITY);

	// Add transform component by default
	auto transform = Factory::CreateInstance<Transform>("Transform", ObjectType::TRANSFORM);
	AddComponent(transform, false);
}

ZObject* Entity::CreateInstance(string name, ObjectType type)
{
	return new Entity(name);
}

ZObject* Entity::Copy(string name, ZObject* object)
{
	if (object == nullptr || object->GetType() != ENTITY)
		return nullptr;

	auto source = static_cast<Entity*>(object);
	auto result = new Entity(name);

	for (auto comp : source->GetAllComponents())
	{
		auto copy = Factory::Copy<Component>(comp->GetName(), comp);

		if (copy != nullptr)
			result->AddComponent(copy, false);
	}

	// Init components after all of them have been added.
	result->InitComponents();

	// Recursively copy children of the entity 
	for (auto child : source->GetTransform()->GetChildren())
	{
		auto childSource = child->GetOwner();
		auto childResult = Factory::Copy<Entity>(childSource->GetName(), childSource);

		childResult->GetTransform()->SetParent(result->GetTransform());
	}

	return result;
}

void Entity::InitComponents()
{
	for (int i = 0; i < _components.size(); i++)
	{
		_components[i]->Init();
	}
}

void Entity::AddComponent(Component* component, bool shouldInit)
{
	// Allow valid components and ONLY one transform component 
	if (component != nullptr)
	{
		if (component->GetType() == TRANSFORM)
		{
			// Remove the previous transform (there can only be one)
			if (_transform != nullptr)
			{
				RemoveComponent(_transform);
			}

			// Keep direct ref to transform (most common access)
			_transform = static_cast<Transform*>(component);
		}

		_components.push_back(component);

		component->SetOwner(this);

		if (shouldInit)
		{
			component->Init();
		}
	}
}

void Entity::RemoveComponent(Component* component)
{
	auto it = find(_components.begin(), _components.end(), component);

	if (it != _components.end())
	{
		auto component = *it;
		component->OnDestroy();

		_components.erase(it);
		delete component;
	}
}

Component* Entity::GetComponent(ObjectType type) const
{
	for (auto c : _components)
	{
		if (c->IsDerivedType(type))
			return c;
	}

	return nullptr;
}

vector<Component*> Entity::GetComponents(ObjectType type) const
{
	std::vector<Component*> results;
	GetComponentsByType(results, type);
	return results;
}

Component* Entity::GetComponentByName(string name) const
{
	auto it = find_if(_components.begin(), _components.end(), [&name](auto c) { return c->GetName() == name; });
	return it != _components.end() ? *it : nullptr;
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
	{
		component->OnDestroy();
	}
	for (auto component : _components)
	{
		delete component;
	}
}
