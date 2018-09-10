#include "Map.h"
#include "Objects/Entity.h"
#include "../Component/Camera.h"

using namespace std;

Map::Map(std::string name) : ZObject(name, ObjectType::MAP)
{
}

ZObject* Map::CreateInstance(std::string name, ObjectType type)
{
	return new Map(name);
}

void Map::Add(Entity* entity)
{
	if (entity != nullptr)
	{
		_entities.push_back(entity);
		
		// If the entity has a camera component, register it
		auto camera = entity->GetComponent(ObjectType::CAMERA);
		if (camera != nullptr)
			_cameras.push_back(static_cast<Camera*>(camera));
	}
}

void Map::Remove(Entity* entity)
{
	auto it = find(_entities.begin(), _entities.end(), entity);

	if (it != _entities.end())
		_entities.erase(it);
}

void Map::Remove(string name)
{
	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		if ((*it)->GetName() == name)
		{
			_entities.erase(it);
			it = _entities.begin();
		}
	}
}

void Map::RegisterCameras()
{
	_cameras.clear();

	for (auto entity : _entities)
	{
		if (entity != nullptr)
		{
			auto camera = entity->GetComponent(ObjectType::CAMERA);

			if (camera != nullptr)
			{
				_cameras.push_back(static_cast<Camera*>(camera));
			}
		}
	}
}

void Map::Render()
{
	
}

Map::~Map()
{
}
