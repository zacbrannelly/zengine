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

