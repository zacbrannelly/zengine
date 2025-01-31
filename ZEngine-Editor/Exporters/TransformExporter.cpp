#include "TransformExporter.h"
#include <ZEngine-Core/Component/Transform.h>

using namespace nlohmann;
using namespace ZEngine;

json TransformExporter::ExportImpl(ZObject* object)
{
	auto transform = static_cast<Transform*>(object);

	json result;

	result["name"] = transform->GetName();
	result["type"] = (int)transform->GetType();

	result["position"] = GetVec3(transform->GetPosition());
	result["scale"] = GetVec3(transform->GetScale());

	// Store in xyzw order.
	auto rotationQuat = transform->GetRotationQuaternion();
	result["rotation"] = GetVec4(glm::vec4(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w));

	return result;
}
