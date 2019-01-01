#pragma once

#include "ComponentExporter.h"

class CameraExporter : public ComponentExporter
{
public:
	nlohmann::json ExportImpl(ZObject* obj) override;
};