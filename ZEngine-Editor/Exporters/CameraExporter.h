#pragma once

#include "ComponentExporter.h"

namespace ZEngine
{
	class CameraExporter : public ComponentExporter
	{
	public:
		nlohmann::json ExportImpl(ZObject* obj) override;
	};
}
