#include "BoxCollider3DImporter.h"
#include "../../Misc/Factory.h"
#include "../BoxCollider3D.h"

using namespace std;
using namespace nlohmann;

ZObject* BoxCollider3DImporter::ImportImpl(string name, json& values)
{
	auto boxCollider3d = Factory::CreateInstance<BoxCollider3D>(name, BOX_COLLIDER_3D);
	boxCollider3d->SetSize(ReadVector<3>("size", values));

	return boxCollider3d;
}
