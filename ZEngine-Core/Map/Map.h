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
	Entity* FindWithComponent(std::string componentName);
	std::vector<Entity*> FindAll(std::string name);
	const std::vector<Entity*>& GetEntities() const;

	void RegisterCameras();
	const std::vector<Camera*>& GetCameras() const;

	void Init();
	void InitScripts();
	void Update();
	void Render();
	void RenderWorld(int viewId);

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);
	static v8::Global<v8::FunctionTemplate> GetTemplate(v8::Isolate* isolate);
private:
	std::vector<Entity*> _entities;
	std::vector<Camera*> _cameras;
};

