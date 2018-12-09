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
	void Release() override;

	Map* GetMap() const;

	static Asset* CreateInstance(std::string name);

private:
	Entity* LoadEntity(nlohmann::json::object_t& entity);

	Map* _map;
};

