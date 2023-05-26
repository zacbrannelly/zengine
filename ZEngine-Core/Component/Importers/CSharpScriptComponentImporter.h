#pragma once

#include "ComponentImporter.h"

class CSharpScriptComponentImporter : public ComponentImporter<CSharpScriptComponentImporter>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override;
};
