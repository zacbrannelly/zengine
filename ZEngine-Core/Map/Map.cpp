#include "Map.h"
#include "Objects/Entity.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Scripting/ScriptSystem.h"
#include "../Misc/Factory.h"

#include <algorithm>

using namespace std;

Map::Map(std::string name) : ZObject(name, ObjectType::MAP)
{
}

ZObject* Map::CreateInstance(std::string name, ObjectType type)
{
	return new Map(name);
}

ZObject* Map::Copy(std::string name, ZObject* object)
{
	if (object == nullptr || object->GetType() != MAP)
		return nullptr;

	auto source = static_cast<Map*>(object);
	auto newMap = new Map(name);

	for (auto entity : source->GetEntities())
	{
		// Only copy root objects since the children get copied (by Entity's Copy func) and added (by Map's Add func).
		if (entity->GetTransform()->GetParent() == nullptr)
		{
			auto copyEntity = Factory::Copy<Entity>(entity->GetName(), entity);
			newMap->Add(copyEntity);
		}
	}

	return newMap;
}

void Map::Init()
{
	for (int i = 0; i < _entities.size(); i++)
	{
		_entities[i]->InitComponents();
	}
}

void Map::InitScripts()
{
	for (int i = 0; i < _entities.size(); i++)
	{
		_entities[i]->InitScripts();
	}
}

void Map::Add(Entity* entity)
{
	if (entity != nullptr)
	{
		_entities.push_back(entity);
		
		// If the entity has a camera component, register it
		auto cameras = entity->GetComponents(ObjectType::CAMERA);
		
		for (auto camera : cameras)
		{
			if (camera != nullptr)
				_cameras.push_back(static_cast<Camera*>(camera));
		}

		// Recursively add child entities to the map too
		if (entity->GetTransform()->GetChildren().size() > 0)
		{
			for (auto child : entity->GetTransform()->GetChildren())
			{
				Add(child->GetOwner());
			}
		}
	}
}

void Map::Remove(Entity* entity)
{
	auto it = find(_entities.begin(), _entities.end(), entity);
	
	if (it != _entities.end())
	{
		// Erase the camera from the cameras vector (if it has one)
		auto cameras = entity->GetComponents(ObjectType::CAMERA);
		if (cameras.size() > 0)
		{
			for (auto camera : cameras)
			{
				auto cameraIt = find(_cameras.begin(), _cameras.end(), camera);

				if (cameraIt != _cameras.end())
					_cameras.erase(cameraIt);
			}
		}

		_entities.erase(it);

		// Recursively delete all the children objects too
		if (entity->GetTransform()->GetChildren().size())
		{
			for (auto child : entity->GetTransform()->GetChildren())
			{
				Remove(child->GetOwner());
			}
		}
	}
}

void Map::Remove(string name)
{
	auto it = find_if(_entities.begin(), _entities.end(), [&name](auto entity) { return entity->GetName() == name; });
	
	while (it != _entities.end())
	{
		_entities.erase(it);
		it = find_if(_entities.begin(), _entities.end(), [&name](auto entity) { return entity->GetName() == name; });
	}
}

Entity* Map::Find(string name)
{
	for (auto entity : _entities)
	{
		if (entity->GetName() == name)
		{
			return entity;
		}
	}

	return nullptr;
}

Entity* Map::FindWithComponent(string componentName)
{
	auto it = find_if(_entities.begin(), _entities.end(), [&componentName](auto e) { return e->GetComponentByName(componentName) != nullptr; });
	return it != _entities.end() ? *it : nullptr;
}

vector<Entity*> Map::FindAll(string name)
{
	vector<Entity*> result;

	for (auto entity : _entities)
	{
		if (entity->GetName() == name)
			result.push_back(entity);
	}

	return result;
}

const std::vector<Entity*>& Map::GetEntities() const
{
	return _entities;
}

void Map::RegisterCameras()
{
	_cameras.clear();

	for (auto entity : _entities)
	{
		if (entity != nullptr)
		{
			auto cameras = entity->GetComponents(ObjectType::CAMERA);

			if (cameras.size() > 0)
			{
				for (auto camera : cameras)
				{
					_cameras.push_back(static_cast<Camera*>(camera));
				}
			}
		}
	}
}

const vector<Camera*>& Map::GetCameras() const
{
	return _cameras;
}

void Map::Update()
{
	for (int i = 0; i < _entities.size(); i++)
	{
		auto entity = _entities[i];
		
		for (int j = 0; j < entity->GetAllComponents().size(); j++)
		{
			entity->GetAllComponents()[j]->Update();
		}
	}
}

void Map::Render()
{
	for (auto camera : _cameras)
	{
		// Render the camera first (negative view id since it doesnt use this input)
		camera->Render(-1);

		// Then render all of the other entities
		RenderWorld(camera->GetViewId());
	}
}

void Map::RenderWorld(int viewId)
{
	for (int i = 0; i < _entities.size(); i++)
	{
		auto entity = _entities[i];

		for (auto component : entity->GetAllComponents())
		{
			if (component->GetType() == ObjectType::CAMERA) continue;

			component->Render(viewId);
		}
	}
}

Map::~Map()
{
	for (auto entity : _entities)
	{
		if (entity != nullptr)
			delete entity;
	}
}

void Callback_Map_Getter(v8::Local<v8::String> nameObj, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	auto sys = ScriptSystem::GetInstance();
	auto name = sys->CastString(nameObj);
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));

	auto scriptableObject = static_cast<IScriptable*>(wrap->Value());
	auto map = static_cast<Map*>(scriptableObject);

	if (map == nullptr)
		return;

	if (name == "name")
	{
		info.GetReturnValue().Set(sys->GetString(map->GetName()));
	}
}

void Callback_Map_Setter(v8::Local<v8::String> nameObj, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	auto sys = ScriptSystem::GetInstance();
	auto name = sys->CastString(nameObj);
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));

	auto scriptableObject = static_cast<IScriptable*>(wrap->Value());
	auto map = static_cast<Map*>(scriptableObject);

	if (map == nullptr)
		return;
}

void Callback_Map_Add(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() == 1 && info[0]->IsObject())
	{
		auto sys = ScriptSystem::GetInstance();
		auto object = info[0]->ToObject(info.GetIsolate());
		auto wrap = v8::Local<v8::External>::Cast(object->GetInternalField(0));
		auto scriptable = static_cast<IScriptable*>(wrap->Value());
		auto entity = static_cast<Entity*>(scriptable);

		if (entity != nullptr)
		{
			wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
			scriptable = static_cast<IScriptable*>(wrap->Value());
			auto map = static_cast<Map*>(scriptable);

			if (map != nullptr)
			{
				map->Add(entity);
			}
		}
	}
}

void Callback_Map_Remove(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() == 1 && info[0]->IsObject())
	{
		auto sys = ScriptSystem::GetInstance();
		auto object = info[0]->ToObject(info.GetIsolate());
		auto wrap = v8::Local<v8::External>::Cast(object->GetInternalField(0));
		auto scriptable = static_cast<IScriptable*>(wrap->Value());
		auto entity = static_cast<Entity*>(scriptable);

		if (entity != nullptr)
		{
			wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
			scriptable = static_cast<IScriptable*>(wrap->Value());
			auto map = static_cast<Map*>(scriptable);

			if (map != nullptr)
			{
				map->Remove(entity);
			}
		}
	}
}

void Callback_Map_Find(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto wrap = v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto map = static_cast<Map*>(scriptable);

	if (map == nullptr || info.Length() != 1 || !info[0]->IsString())
	{
		info.GetReturnValue().SetNull();
		return;
	}

	auto objName = ScriptSystem::GetInstance()->CastString(info[0]->ToString(info.GetIsolate()));
	auto obj = map->Find(objName);

	if (obj != nullptr)
	{
		info.GetReturnValue().Set(obj->GetScriptObject());
	}
	else
	{
		info.GetReturnValue().SetNull();
	}
}

v8::Global<v8::FunctionTemplate> Map::GetTemplate(v8::Isolate* isolate)
{
	using namespace v8;
	auto sys = ScriptSystem::GetInstance();
	auto constructor = FunctionTemplate::New(isolate);

	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->InstanceTemplate()->SetAccessor(sys->GetString("name"), Callback_Map_Getter, Callback_Map_Setter);
	constructor->InstanceTemplate()->Set(isolate, "Add", FunctionTemplate::New(isolate, Callback_Map_Add));
	constructor->InstanceTemplate()->Set(isolate, "Remove", FunctionTemplate::New(isolate, Callback_Map_Remove));
	constructor->InstanceTemplate()->Set(isolate, "Find", FunctionTemplate::New(isolate, Callback_Map_Find));

	return v8::Global<FunctionTemplate>(isolate, constructor);
}
