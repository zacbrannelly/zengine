#include "MapManager.h"
#include "Map.h"

MapManager::MapManager() : Singleton<MapManager>()
{
	_currentMap = nullptr;
	_prevMap = nullptr;
}

void MapManager::SetCurrentMap(Map* currentMap)
{
	_prevMap = _currentMap;

	_currentMap = currentMap;

	if (_currentMap != nullptr)
	{
		_currentMap->Init();
		_currentMap->InitScripts();
	}
}

Map* MapManager::GetCurrentMap() const
{
	return _currentMap;
}

Map* MapManager::GetPreviousMap() const
{
	return _prevMap;
}
