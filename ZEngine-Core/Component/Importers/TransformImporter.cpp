#include "TransformImporter.h"
#include "../../Misc/Factory.h"
#include "../Transform.h"

using namespace std;
using namespace nlohmann;

ZObject* TransformImporter::ImportImpl(string name, json::object_t& values)
{
	auto transform = Factory::CreateInstance<Transform>(name, TRANSFORM);

	transform->SetPosition(ReadVector<3>("position", values));
	transform->SetRotation(ReadVector<3>("rotation", values));
	transform->SetScale(ReadVector<3>("scale", values));

	return transform;
}