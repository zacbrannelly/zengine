#pragma once

#include "../Asset.h"
#include "../../../json/json.hpp"

class Map;
class Entity;
class Component;

class MapAsset : public Asset
{
public:
	MapAsset(std::string name);
	~MapAsset();

	bool Load(std::string path) override;

	Map* GetMap() const;

private:
	Entity* LoadEntity(nlohmann::json::object_t& entity);

	Map* _map;
};

