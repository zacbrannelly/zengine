#include "Entity.h"
#include "../../Component/Component.h"
#include "../../Misc/Factory.h"
#include "../../Component/Transform.h"
#include "../../Scripting/ScriptSystem.h"
#include "../../Scripting/IScriptable.h"
#include "../../Component/ScriptComponent.h"

using namespace std;

Entity::Entity(string name) : ZObject(name, ObjectType::ENTITY)
{	
	// Add transform component by default
	auto transform = Factory::CreateInstance<Transform>("Transform", ObjectType::TRANSFORM);
	AddComponent(transform);
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
			result->AddComponent(copy, true);
	}

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

void Entity::InitScripts()
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components[i]->GetType() == SCRIPT_COMPONENT)
			static_cast<ScriptComponent*>(_components[i])->InitScript();
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

			// Update script transform field with new transform (TODO: find a better solution to this, e.g. keep the same script object and just change internal field)
			auto scriptSystem = ScriptSystem::GetInstance();
			auto scriptObject = GetScriptObject();
			v8::Context::Scope scope(scriptSystem->GetContext()->GetLocal());
			scriptObject->Set(scriptSystem->GetString("transform"), _transform->GetScriptObject());
		}

		_components.push_back(component);

		component->SetOwner(this);

		if (shouldInit)
		{
			component->Init();
			
			if (component->GetType() == SCRIPT_COMPONENT)
				static_cast<ScriptComponent*>(component)->InitScript();
		}
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
		delete component;
}

void Callback_Entity_AddComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto entity = static_cast<Entity*>(scriptable);

	if (entity == nullptr || info.Length() != 1 || !info[0]->IsObject())
		return;

	auto object = info[0]->ToObject(info.GetIsolate());
	wrap = v8::Local<v8::External>::Cast(object->GetInternalField(0));
	scriptable = static_cast<IScriptable*>(wrap->Value());
	auto component = static_cast<Component*>(scriptable);

	if (component != nullptr)
		entity->AddComponent(component, true);
}

void Callback_Entity_RemoveComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto entity = static_cast<Entity*>(scriptable);

	if (entity == nullptr || info.Length() != 1 || !info[0]->IsObject())
		return;

	auto object = info[0]->ToObject(info.GetIsolate());
	wrap = v8::Local<v8::External>::Cast(object->GetInternalField(0));
	scriptable = static_cast<IScriptable*>(wrap->Value());
	auto component = static_cast<Component*>(scriptable);

	if (component != nullptr)
		entity->RemoveComponent(component);
}

void Callback_Entity_GetComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto entity = static_cast<Entity*>(scriptable);

	if (entity == nullptr || info.Length() != 1 || !info[0]->IsString())
		return;

	auto componentName = ScriptSystem::GetInstance()->CastString(info[0]->ToString(info.GetIsolate()));
	auto component = entity->GetComponentByName(componentName);
	
	if (component != nullptr)
	{
		info.GetReturnValue().Set(component->GetScriptObject());
	}
	else
	{
		info.GetReturnValue().SetNull();
	}
}

void Callback_Entity_Getter(v8::Local<v8::String> nameObj, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	auto sys = ScriptSystem::GetInstance();
	auto name = sys->CastString(nameObj);

	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto entity = static_cast<Entity*>(scriptable);

	if (entity == nullptr)
		return;

	if (name == "name")
	{
		info.GetReturnValue().Set(sys->GetString(entity->GetName()));
	}
	else if (name == "transform")
	{
		info.GetReturnValue().Set(entity->GetTransform()->GetScriptObject());
	}
}

void Callback_Entity_Setter(v8::Local<v8::String> nameObj, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	auto sys = ScriptSystem::GetInstance();
	auto name = sys->CastString(nameObj);

	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto entity = static_cast<Entity*>(scriptable);

	if (entity == nullptr)
		return;
}

v8::Global<v8::FunctionTemplate> Entity::GetTemplate(v8::Isolate* isolate)
{
	using namespace v8;

	auto sys = ScriptSystem::GetInstance();

	auto constructor = FunctionTemplate::New(isolate);
	constructor->SetClassName(sys->GetString("Entity"));

	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->InstanceTemplate()->SetAccessor(sys->GetString("name"), Callback_Entity_Getter, Callback_Entity_Setter);
	constructor->InstanceTemplate()->SetAccessor(sys->GetString("transform"), Callback_Entity_Getter, Callback_Entity_Setter);
	constructor->InstanceTemplate()->Set(isolate, "AddComponent", FunctionTemplate::New(isolate, Callback_Entity_AddComponent));
	constructor->InstanceTemplate()->Set(isolate, "RemoveComponent", FunctionTemplate::New(isolate, Callback_Entity_RemoveComponent));
	constructor->InstanceTemplate()->Set(isolate, "GetComponent", FunctionTemplate::New(isolate, Callback_Entity_GetComponent));

	return v8::Global<FunctionTemplate>(isolate, constructor);
}
