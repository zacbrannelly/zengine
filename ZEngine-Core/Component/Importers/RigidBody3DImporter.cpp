#include "RigidBody3DImporter.h"
#include "../../Misc/Factory.h"
#include "../RigidBody3D.h"

using namespace std;
using namespace nlohmann;

ZObject* RigidBody3DImporter::ImportImpl(string name, json& values)
{
	auto rigidBody3D = Factory::CreateInstance<RigidBody3D>(name, RIGID_BODY_3D);
	// TODO

	return rigidBody3D;
}
