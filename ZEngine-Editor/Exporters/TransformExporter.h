#pragma once

#include "ComponentExporter.h"

namespace ZEngine
{
	class TransformExporter : public ComponentExporter
	{
	public:
		nlohmann::json ExportImpl(ZObject* object) override;
	};
}
