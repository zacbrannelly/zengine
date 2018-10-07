#pragma once

#include "../Asset.h"
#include "../../../json/json.hpp"

class Map;

class MapAsset : public Asset
{
public:
	MapAsset(std::string name);
	~MapAsset();

	bool Load(std::string path) override;

	Map* GetMap() const;

private:
	Map* _map;
};

