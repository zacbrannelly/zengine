#include "MapAsset.h"
#include "../AssetManager.h"
#include "../AssetCatalog.h"
#include "../../Map/Map.h"
#include "../../Map/Objects/Entity.h"
#include "../../Component/Component.h"
#include "../../Component/Transform.h"
#include "../../Misc/Factory.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace nlohmann;

MapAsset::MapAsset(string name) : Asset(name, ObjectType::MAP_ASSET)
{
}

Asset* MapAsset::CreateInstance(string name)
{
	return new MapAsset(name);
}

bool MapAsset::Load(string path)
{
	ifstream in(path, ios::in);

	if (!in.is_open())
	{
		cout << "MAP_ASSET: Failed to load file: " << path << endl;
		return false;
	}

	json root;
	root << in;

	in.close();

	auto it = root.find("name");
	if (it != root.end() && (*it).is_string())
	{
		_map = new Map((*it).get<string>());
	}
	else
	{
		cout << "MAP_ASSET: Invalid map file, no name specified!" << endl;
		return false;
	}

	it = root.find("entities");
	if (it != root.end() && (*it).is_array())
	{
		auto entitiesArray = (*it).get<json::array_t>();

		for (auto value : entitiesArray)
		{
			if (value.is_object())
			{
				auto entityObj = value.get<json::object_t>();
				auto entity = LoadEntity(entityObj);

				if (entity != nullptr)
					_map->Add(entity);
			}
		}
	}

	return true;
}

Entity* MapAsset::LoadEntity(json::object_t& object)
{
	Entity* entity = nullptr;
	Transform* transform = nullptr;

	auto name = object.at("name");
	auto components = object.at("components");

	if (name.is_string() && components.is_array())
	{
		entity = new Entity(name.get<string>());
	}

	if (entity == nullptr)
		return nullptr;

	for (auto value : components)
	{
		if (value.is_object())
		{
			// Use an importer in the factory to load the component from the json object
			auto newComp = Factory::ImportInstance<Component>(value.get<json::object_t>());

			if (newComp != nullptr)
				entity->AddComponent(newComp);
		}
	}

	auto it = object.find("children");
	if (it != object.end() && (*it).second.is_array())
	{
		for (auto& child : (*it).second.get<json::array_t>())
		{
			if (child.is_object())
			{
				auto childEntity = LoadEntity(child.get<json::object_t>());

				if (childEntity != nullptr)
				{
					auto transform = static_cast<Transform*>(childEntity->GetComponent(TRANSFORM));
					transform->SetParent(entity->GetTransform());

					_map->Add(childEntity);
				}
			}
		}
	}

	return entity;
}

Map* MapAsset::GetMap() const
{
	return _map;
}

MapAsset::~MapAsset()
{
}
