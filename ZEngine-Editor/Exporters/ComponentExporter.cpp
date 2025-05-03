#include "ComponentExporter.h"

#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Component/MeshRenderer.h>
#include <ZEngine-Core/Component/AudioSource.h>
#include <ZEngine-Core/Component/BoxCollider3D.h>
#include <ZEngine-Core/Component/CapsuleCollider3D.h>
#include <ZEngine-Core/Component/SphereCollider3D.h>
#include <ZEngine-Core/Component/PlaneCollider3D.h>
#include <ZEngine-Core/Component/MeshCollider3D.h>
#include <ZEngine-Core/Component/RigidBody3D.h>
#include <ZEngine-Core/Component/CSharpScriptComponent.h>
#include <ZEngine-Core/Component/Lighting/DirectionalLight.h>
#include <ZEngine-Core/Component/Lighting/PointLight.h>
#include <ZEngine-Core/Component/Lighting/SpotLight.h>

using namespace nlohmann;
using namespace ZEngine;

std::map<ObjectType, ComponentExporter*> ComponentExporter::_exporters;

template<typename T>
class DefaultExporter : public ComponentExporter
{
public:
	virtual json ExportImpl(ZObject* object) override
	{
		auto component = static_cast<T*>(object);

		// Use the JSON schema defined in the component to export the data
		json result;
		to_json(result, *component);

		// Add the name and type to the JSON object
		result["name"] = component->GetName();
		result["type"] = ObjectTypeToString(component->GetType());

		return result;
	}
};

#define REGISTER_DEFAULT_EXPORTER(type) RegisterExporter(new DefaultExporter<type>(), type::GetStaticType())

void ComponentExporter::RegisterAllTypes()
{
	REGISTER_DEFAULT_EXPORTER(Transform);
	REGISTER_DEFAULT_EXPORTER(Camera);
	REGISTER_DEFAULT_EXPORTER(MeshRenderer);
	REGISTER_DEFAULT_EXPORTER(AudioSource);
	REGISTER_DEFAULT_EXPORTER(BoxCollider3D);
	REGISTER_DEFAULT_EXPORTER(CapsuleCollider3D);
	REGISTER_DEFAULT_EXPORTER(SphereCollider3D);
	REGISTER_DEFAULT_EXPORTER(PlaneCollider3D);
	REGISTER_DEFAULT_EXPORTER(MeshCollider3D);
	REGISTER_DEFAULT_EXPORTER(RigidBody3D);
	REGISTER_DEFAULT_EXPORTER(CSharpScriptComponent);
	REGISTER_DEFAULT_EXPORTER(DirectionalLight);
	REGISTER_DEFAULT_EXPORTER(PointLight);
	REGISTER_DEFAULT_EXPORTER(SpotLight);
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
