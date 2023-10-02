#pragma once

#include "ComponentImporter.h"

class SphereCollider3DImporter : public ComponentImporter<SphereCollider3DImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;
};
