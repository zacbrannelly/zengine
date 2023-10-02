#pragma once

#include "ComponentImporter.h"

class CapsuleCollider3DImporter : public ComponentImporter<CapsuleCollider3DImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;
};
