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
#include "../Component/ScriptComponent.h"
#include "../Scripting/Script.h"

#include "../Component/Importers/TransformImporter.h"
#include "../Component/Importers/CameraImporter.h"
#include "../Component/Importers/MeshRendererImporter.h"
#include "../Component/Importers/ScriptComponentImporter.h"

std::map<ObjectType, ConstructorFunc> Factory::_typeConstructors;
std::map<ObjectType, ImporterFunc> Factory::_importers;

void Factory::Init()
{
	// Register instantiators (real-time creation)
	RegisterType(ObjectType::ENTITY, &Entity::CreateInstance);
	RegisterType(ObjectType::MAP, &Map::CreateInstance);
	RegisterType(ObjectType::MATERIAL, &Material::CreateInstance);
	RegisterType(ObjectType::MESH, &Mesh::CreateInstance);
	RegisterType(ObjectType::SHADER, &Shader::CreateInstance);
	RegisterType(ObjectType::CAMERA, &Camera::CreateInstance);
	RegisterType(ObjectType::TRANSFORM, &Transform::CreateInstance);
	RegisterType(ObjectType::MESH_RENDERER, &MeshRenderer::CreateInstance);
	RegisterType(ObjectType::SCRIPT, &Script::CreateInstance);
	RegisterType(ObjectType::SCRIPT_COMPONENT, &ScriptComponent::CreateInstance);
	RegisterType(ObjectType::TEST_RENDERER, &TestRenderer::CreateInstance);

	// Register importers (from JSON objects)
	TransformImporter::Init();
	RegisterTypeImporter(ObjectType::TRANSFORM, TransformImporter::Import);

	CameraImporter::Init();
	RegisterTypeImporter(ObjectType::CAMERA, CameraImporter::Import);

	MeshRendererImporter::Init();
	RegisterTypeImporter(ObjectType::MESH_RENDERER, MeshRendererImporter::Import);

	ScriptComponentImporter::Init();
	RegisterTypeImporter(ObjectType::SCRIPT_COMPONENT, ScriptComponentImporter::Import);
}

void Factory::RegisterType(ObjectType type, ConstructorFunc constructor)
{
	_typeConstructors[type] = constructor;
}

void Factory::RegisterTypeImporter(ObjectType type, ImporterFunc importer)
{
	_importers[type] = importer;
}