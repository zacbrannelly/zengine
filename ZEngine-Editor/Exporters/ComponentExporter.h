#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

#include <ZEngine-Core/Map/Objects/ZObject.h>

namespace ZEngine
{
	class ComponentExporter
	{
	public:
		virtual nlohmann::json ExportImpl(ZObject* obj) = 0;

		nlohmann::json::array_t GetVec2(const glm::vec2& vec);
		nlohmann::json::array_t GetVec3(const glm::vec3& vec);
		nlohmann::json::array_t GetVec4(const glm::vec4& vec);

		static void RegisterAllTypes();
		static void Cleanup();
		static void RegisterExporter(ComponentExporter* exporter, ObjectType type);
		static nlohmann::json Export(ZObject* object);
	private:
		static std::map<ObjectType, ComponentExporter*> _exporters;
	};
}