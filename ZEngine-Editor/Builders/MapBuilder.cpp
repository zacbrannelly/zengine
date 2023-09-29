#include "MapBuilder.h"
#include "../Exporters/ComponentExporter.h"
#include <ZEngine-Core/Utilities/Directory.h>
#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Component/Component.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <nlohmann/json.hpp>

using namespace nlohmann;

bool MapBuilder::BuildToFile(const std::string& path, const Map* map)
{
	json root;

	// Setup name and entities array 
	root["name"] = map->GetName();
	auto& entities = root["entities"] = json::array_t();

	for (const auto& entity : map->GetEntities())
	{
		auto newEntityObj = json::object_t();
		newEntityObj["name"] = entity->GetName();

		auto& components = newEntityObj["components"] = json::array_t();

		// Use ComponentExporter to create JSON objects for each comp
		for (const auto& component : entity->GetAllComponents())
		{
			auto componentObj = ComponentExporter::Export(component);

			if (!componentObj.is_null())
				components.push_back(componentObj.get<json::object_t>());
		}

		entities.push_back(newEntityObj);
	}

	// Export the JSON code to the map file
	File mapFile(path);
	auto data = root.dump(4);

	return mapFile.WriteContent(data.data(), data.size());
}

bool MapBuilder::BuildDefault(const std::string& path, const std::string& name)
{
	// Create empty map
	auto defaultMap = Factory::CreateInstance<Map>(name, MAP);

	// Create empty entity with camera attached
	auto cameraObj = Factory::CreateInstance<Entity>("Main Camera", ENTITY);
	auto cameraComp = Factory::CreateInstance<Camera>("Camera", CAMERA);
	cameraObj->AddComponent(cameraComp, false);
	defaultMap->Add(cameraObj);

	// Save map to file 
	auto success = BuildToFile(path, defaultMap);

	// Clean up memory
	delete defaultMap;

	return success;
}