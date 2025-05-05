#pragma once

#include <string>

namespace ZEngine
{
	class Map;
	class MapAsset;

	class MapBuilder
	{
	public:
		static bool BuildDefault(const std::string& path, const std::string& name);
		static bool BuildToFile(const std::string& path, const Map* map);
		static bool BuildToFile(const MapAsset* mapAsset);
	};
}
