#pragma once

#include "ComponentImporter.h"

class CameraImporter : public ComponentImporter<CameraImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json::object_t& values) override;
};
