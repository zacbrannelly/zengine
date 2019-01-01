#pragma once

#include <string>

class Map;

class MapBuilder
{
public:
	static bool BuildDefault(const std::string& path, const std::string& name);
	static bool BuildToFile(const std::string& path, const Map* map);
};