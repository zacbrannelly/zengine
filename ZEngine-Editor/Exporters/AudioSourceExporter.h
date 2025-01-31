#pragma once

#include "ComponentExporter.h"

namespace ZEngine
{
	class AudioSourceExporter : public ComponentExporter
	{
	public:
		nlohmann::json ExportImpl(ZObject* obj) override;
	};
}
