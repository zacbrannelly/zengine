#include "SphereCollider3DImporter.h"
#include "../../Misc/Factory.h"
#include "../SphereCollider3D.h"

using namespace std;
using namespace nlohmann;

ZObject* SphereCollider3DImporter::ImportImpl(string name, json& values)
{
	auto collider = Factory::CreateInstance<SphereCollider3D>(name, SPHERE_COLLIDER_3D);
	collider->SetIsTrigger(ReadBool("isTrigger", values));
	collider->SetRadius(ReadFloat("radius", values));

	return collider;
}
