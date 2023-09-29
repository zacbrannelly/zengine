#pragma once

#include "ComponentImporter.h"

class RigidBody3DImporter : public ComponentImporter<RigidBody3DImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;
};
