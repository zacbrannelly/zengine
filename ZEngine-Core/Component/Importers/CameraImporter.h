#pragma once

#include "ComponentImporter.h"

class CameraImporter : public ComponentImporter<CameraImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;
};

