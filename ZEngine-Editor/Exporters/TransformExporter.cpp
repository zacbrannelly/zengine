#include "TransformExporter.h"
#include <ZEngine-Core/Component/Transform.h>

using namespace nlohmann;

json TransformExporter::ExportImpl(ZObject* object)
{
	auto transform = static_cast<Transform*>(object);

	json result;

	result["name"] = transform->GetName();
	result["type"] = (int)transform->GetType();

	result["position"] = GetVec3(transform->GetPosition());
	result["rotation"] = GetVec3(transform->GetRotation());
	result["scale"] = GetVec3(transform->GetScale());

	return result;
}