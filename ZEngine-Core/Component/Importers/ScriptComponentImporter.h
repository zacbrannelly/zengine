#pragma once

#include "ComponentImporter.h"

class ScriptComponentImporter : public ComponentImporter<ScriptComponentImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json::object_t& values) override;
};

