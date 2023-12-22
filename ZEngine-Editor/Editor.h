#pragma once
#include <future>
#include "UI/Container.h"
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Misc/Singleton.h>

class Project;
class Display;
class MapController;

class Editor : public Container, public Singleton<Editor>
{
public:
	Editor(Display* display);
	~Editor();

	void Update() override;
	void RequestClose() const;

	void SetProject(Project* project);
	Project* GetProject() const;

	void SetSelectedMap(Map* map);
	Map* GetSelectedMap() const;

	void SetSelectedEntity(Entity* entity);
	Entity* GetSelectedEntity() const;

	MapController* GetMapController() const;

private:
	Display* _display;
	Project* _project;
	Map* _selectedMap;
	Entity* _selectedObject;

	MapController* _mapController;

	std::future<bool> _buildFuture;
};
