#pragma once

#include "ComponentExporter.h"

class ScriptComponentExporter : public ComponentExporter
{
public:
	nlohmann::json ExportImpl(ZObject* obj) override;
};
