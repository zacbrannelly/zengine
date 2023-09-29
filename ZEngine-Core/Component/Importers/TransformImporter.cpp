#include "TransformImporter.h"
#include "../../Misc/Factory.h"
#include "../Transform.h"

using namespace std;
using namespace nlohmann;

ZObject* TransformImporter::ImportImpl(string name, json& values)
{
	auto transform = Factory::CreateInstance<Transform>(name, TRANSFORM);

	transform->SetPosition(ReadVector<3>("position", values));
	transform->SetScale(ReadVector<3>("scale", values));

	// Stored in xyzw order.
	auto rotationQuat = ReadVector<4>("rotation", values);
	transform->SetRotationQuaternion(glm::quat(rotationQuat[3], rotationQuat[0], rotationQuat[1], rotationQuat[2]));

	return transform;
}