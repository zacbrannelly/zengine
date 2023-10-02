#include "CapsuleCollider3DImporter.h"
#include "../../Misc/Factory.h"
#include "../CapsuleCollider3D.h"

using namespace std;
using namespace nlohmann;

ZObject* CapsuleCollider3DImporter::ImportImpl(string name, json& values)
{
	auto collider = Factory::CreateInstance<CapsuleCollider3D>(name, CAPSULE_COLLIDER_3D);
	collider->SetSize(ReadFloat("radius", values), ReadFloat("height", values));

	return collider;
}
