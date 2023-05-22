#pragma once

#include "ComponentExporter.h"

class AudioSourceExporter : public ComponentExporter
{
public:
	nlohmann::json ExportImpl(ZObject* obj) override;
};