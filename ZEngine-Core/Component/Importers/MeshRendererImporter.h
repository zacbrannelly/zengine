#pragma once

#include "ComponentImporter.h"

class MeshRendererImporter : public ComponentImporter<MeshRendererImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json::object_t& values) override;
};

