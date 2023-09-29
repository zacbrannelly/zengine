#pragma once

#include "ComponentImporter.h"

class BoxCollider3DImporter : public ComponentImporter<BoxCollider3DImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;
};
