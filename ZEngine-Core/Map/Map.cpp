#include "Map.h"
#include "Objects/Entity.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"

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
	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		if ((*it)->GetName() == name)
		{
			_entities.erase(it);
			it = _entities.begin();
		}
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

const std::vector<Entity*>& Map::GetEntities()
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
	for (auto entity : _entities)
	{
		for (auto component : entity->GetAllComponents())
		{
			component->Update();
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
	for (auto entity : _entities)
	{
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
