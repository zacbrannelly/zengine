#include "Cube.h"

#include "../../../Component/MeshRenderer.h"
#include "../../../Rendering/Mesh.h"
#include "../../../Rendering/MeshFactory.h"
#include "../../../Rendering/Material.h"
#include "../../../Rendering/Shader.h"
#include "../../../Misc/Factory.h"

using namespace ZEngine;

ZObject* Cube::CreateInstance(std::string name, ObjectType type)
{
	auto newEntity = new Entity(name);

	auto cubeMesh = MeshFactory::CreateCube(name + "_mesh");
	//auto shader = Factory::CreateInstance<Shader>(name + "_shader", ObjectType::SHADER);

	return newEntity;
}

