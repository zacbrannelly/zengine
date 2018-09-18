#include "Entity.h"
#include "../../Component/Component.h"
#include "../../Misc/Factory.h"
#include "../../Component/Transform.h"
#include "../../Scripting/ScriptSystem.h"
#include "../../Scripting/IScriptable.h"

using namespace std;

Entity::Entity(string name) : ZObject(name, ObjectType::ENTITY), IScriptable(ObjectType::ENTITY)
{
	// Add transform component by default
	_transform = Factory::CreateInstance<Transform>("Transform", ObjectType::TRANSFORM);
	AddComponent(_transform);
	
	auto scriptSystem = ScriptSystem::GetInstance();
	auto scriptObject = GetScriptObject();
	v8::Context::Scope scope(scriptSystem->GetContext()->GetLocal());

	//TODO: Add extra script functionality here (alive data)
	scriptObject->Set(scriptSystem->GetString("transform"), _transform->GetScriptObject());
}

ZObject* Entity::CreateInstance(string name, ObjectType type)
{
	return new Entity(name);
}

v8::Global<v8::FunctionTemplate> Entity::GetTemplate(v8::Isolate* isolate)
{
	using namespace v8;

	auto sys = ScriptSystem::GetInstance();

	auto constructor = FunctionTemplate::New(isolate);
	constructor->SetClassName(sys->GetString("Entity"));

	constructor->InstanceTemplate()->SetInternalFieldCount(1);

	//TODO: Add properties (getters/setters only)

	return v8::Global<FunctionTemplate>(isolate, constructor);
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

		//TODO: Send message to component that it has been removed
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
