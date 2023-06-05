#include "ComponentExporter.h"
#include "AudioSourceExporter.h"
#include "MeshRendererExporter.h"
#include "TransformExporter.h"
#include "CameraExporter.h"

using namespace nlohmann;

std::map<ObjectType, ComponentExporter*> ComponentExporter::_exporters;

void ComponentExporter::RegisterAllTypes()
{
	RegisterExporter(new TransformExporter(), TRANSFORM);
	RegisterExporter(new CameraExporter(), CAMERA);
	RegisterExporter(new MeshRendererExporter(), MESH_RENDERER);
	RegisterExporter(new AudioSourceExporter(), AUDIO_SOURCE);
}

void ComponentExporter::RegisterExporter(ComponentExporter* exporter, ObjectType type)
{
	_exporters[type] = exporter;
}

void ComponentExporter::Cleanup()
{
	for (auto& pair : _exporters)
	{
		if (pair.second != nullptr)
			delete pair.second;
	}
}

json ComponentExporter::Export(ZObject* object)
{
	auto it = _exporters.find(object->GetType());

	if (it != _exporters.end() && it->second != nullptr)
		return it->second->ExportImpl(object);

	return json();
}

json::array_t ComponentExporter::GetVec2(const glm::vec2& vec)
{
	return { vec.x, vec.y };
}

json::array_t ComponentExporter::GetVec3(const glm::vec3& vec)
{
	return { vec.x, vec.y, vec.z };
}

json::array_t ComponentExporter::GetVec4(const glm::vec4& vec)
{
	return { vec.x, vec.y, vec.z, vec.w };
}