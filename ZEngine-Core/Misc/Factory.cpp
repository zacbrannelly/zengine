#include "Factory.h"

#include "../Map/Objects/Entity.h"
#include "../Map/Map.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Material.h"
#include "../Rendering/Mesh.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Component/TestRenderer.h"

std::map<ObjectType, ZObject*(*)(std::string, ObjectType)> Factory::_typeConstructors;

void Factory::Init()
{
	RegisterType(ObjectType::ENTITY, &Entity::CreateInstance);
	RegisterType(ObjectType::MAP, &Map::CreateInstance);
	RegisterType(ObjectType::MATERIAL, &Material::CreateInstance);
	RegisterType(ObjectType::MESH, &Mesh::CreateInstance);
	RegisterType(ObjectType::SHADER, &Shader::CreateInstance);
	RegisterType(ObjectType::CAMERA, &Camera::CreateInstance);
	RegisterType(ObjectType::TRANSFORM, &Transform::CreateInstance);
	RegisterType(ObjectType::MESH_RENDERER, &MeshRenderer::CreateInstance);
	RegisterType(ObjectType::TEST_RENDERER, &TestRenderer::CreateInstance);
}

void Factory::RegisterType(ObjectType type, ConstructorFunc constructor)
{
	_typeConstructors[type] = constructor;
}