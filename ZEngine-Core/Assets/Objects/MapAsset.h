#pragma once

#include "../Asset.h"
#include "../../Physics/Physics3DSystem.h"
#include <nlohmann/json.hpp>

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
	
	static ObjectType GetStaticType()
	{
		return MAP_ASSET;
	}

private:
	PhysicsSceneDescription LoadPhysicsSceneDescription(nlohmann::json& physics);
	Entity* LoadEntity(nlohmann::json& entity);

	Map* _map;
};

