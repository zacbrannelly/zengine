#pragma once

#include <vector>
#include "Objects/ZObject.h"

class Entity;
class Camera;

class Map : public ZObject
{
public:
	Map(std::string name);
	~Map();

	void Add(Entity* entity);
	void Remove(Entity* entity);
	void Remove(std::string name);

	Entity* Find(std::string name);
	std::vector<Entity*> FindAll(std::string name);
	const std::vector<Entity*>& GetEntities();

	void RegisterCameras();
	const std::vector<Camera*>& GetCameras() const;

	void Update();
	void Render();
	void RenderWorld(int viewId);

	static ZObject* CreateInstance(std::string name, ObjectType type);
private:
	std::vector<Entity*> _entities;
	std::vector<Camera*> _cameras;
};

