#pragma once

#include <map>
#include <nlohmann/json.hpp>

#include <ZEngine-Core/Map/Objects/ZObject.h>

namespace ZEngine
{
	class ComponentExporter
	{
	public:
		ComponentExporter() = default;
		virtual ~ComponentExporter() = default;
		
		virtual nlohmann::json ExportImpl(ZObject* obj) = 0;

		static void RegisterAllTypes();
		static void Cleanup();
		static void RegisterExporter(ComponentExporter* exporter, ObjectType type);
		static nlohmann::json Export(ZObject* object);
	private:
		static std::map<ObjectType, ComponentExporter*> _exporters;
	};
}
