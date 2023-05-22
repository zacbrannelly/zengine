#pragma once

#include "ComponentExporter.h"

class TransformExporter : public ComponentExporter
{
public:
	nlohmann::json ExportImpl(ZObject* object) override;
};