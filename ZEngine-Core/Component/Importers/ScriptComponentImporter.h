#pragma once

#include "ComponentImporter.h"

class ScriptComponentImporter : public ComponentImporter<ScriptComponentImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;
};

