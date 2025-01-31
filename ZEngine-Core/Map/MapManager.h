#pragma once

#include "../Misc/Singleton.h"

namespace ZEngine
{
	class Map;

	class MapManager : public Singleton<MapManager>
	{
	public:
		MapManager();

		void SetCurrentMap(Map* map);
		Map* GetCurrentMap() const;
		Map* GetPreviousMap() const;

	private:
		Map* _currentMap;
		Map* _prevMap;
	};
}
