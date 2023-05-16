#pragma once

#include "ComponentImporter.h"

class MeshRendererImporter : public ComponentImporter<MeshRendererImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;
};

