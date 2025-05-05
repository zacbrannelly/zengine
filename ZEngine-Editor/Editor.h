#pragma once
#include <future>
#include <ZEngine-Core/ImmediateUI/Container.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>

namespace ZEngine
{
	class Project;
	class Display;
	class MapController;
	class MapAsset;

	class Editor : public Container
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

		void SetSelectedMapAsset(MapAsset* mapAsset);
		MapAsset* GetSelectedMapAsset() const;
		void SaveSelectedMapAsset();

		void SetSelectedEntity(Entity* entity);
		Entity* GetSelectedEntity() const;

		MapController* GetMapController() const;
		Display* GetDisplay() const;

	private:
		Display* _display;
		Project* _project;
		Map* _selectedMap;
		MapAsset* _selectedMapAsset;
		Entity* _selectedObject;

		MapController* _mapController;

		std::future<bool> _buildFuture;
	};
}
