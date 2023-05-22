#pragma once
#include "UI/Container.h"
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>

class Editor : public Container
{
public:
	Editor();
	~Editor();

	void Update();

	void SetSelectedMap(Map* map);
	Map* GetSelectedMap() const;

	void SetSelectedEntity(Entity* entity);
	Entity* GetSelectedEntity() const;

private:
	Map* _selectedMap;
	Entity* _selectedObject;
};