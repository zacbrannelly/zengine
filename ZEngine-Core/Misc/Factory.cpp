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
#include "../Component/CSharpScriptComponent.h"
#include "../Component/AudioSource.h"

#include "../Scripting/CSharp/CSharpScript.h"

#include "../Component/Importers/TransformImporter.h"
#include "../Component/Importers/CameraImporter.h"
#include "../Component/Importers/MeshRendererImporter.h"
#include "../Component/Importers/CSharpScriptComponentImporter.h"
#include "../Component/Importers/AudioSourceImporter.h"

#include <iostream>

std::map<ObjectType, ConstructorFunc> Factory::_typeConstructors;
std::map<ObjectType, CopyFunc> Factory::_copyFunctions;
std::map<ObjectType, ImporterFunc> Factory::_importers;

void Factory::Init()
{
	// Register factory methods (real-time creation)
	RegisterType(ObjectType::ENTITY, &Entity::CreateInstance);
	RegisterType(ObjectType::MAP, &Map::CreateInstance);
	RegisterType(ObjectType::MATERIAL, &Material::CreateInstance);
	RegisterType(ObjectType::MESH, &Mesh::CreateInstance);
	RegisterType(ObjectType::SHADER, &Shader::CreateInstance);
	RegisterType(ObjectType::CAMERA, &Camera::CreateInstance);
	RegisterType(ObjectType::TRANSFORM, &Transform::CreateInstance);
	RegisterType(ObjectType::MESH_RENDERER, &MeshRenderer::CreateInstance);
	RegisterType(ObjectType::CSHARP_SCRIPT, &CSharpScript::CreateInstance);
	RegisterType(ObjectType::CSHARP_SCRIPT_COMPONENT, &CSharpScriptComponent::CreateInstance);
	RegisterType(ObjectType::AUDIO_SOURCE, &AudioSource::CreateInstance);
	RegisterType(ObjectType::TEST_RENDERER, &TestRenderer::CreateInstance);

	// Register copy methods (real-time copying)
	RegisterCopyType(ObjectType::MAP, &Map::Copy);
	RegisterCopyType(ObjectType::ENTITY, &Entity::Copy);
	RegisterCopyType(ObjectType::TRANSFORM, &Transform::Copy);
	RegisterCopyType(ObjectType::MESH_RENDERER, &MeshRenderer::Copy);
	RegisterCopyType(ObjectType::CAMERA, &Camera::Copy);
	RegisterCopyType(ObjectType::CSHARP_SCRIPT_COMPONENT, &CSharpScriptComponent::Copy);
	RegisterCopyType(ObjectType::AUDIO_SOURCE, &AudioSource::Copy);

	// Register importers (from JSON objects)
	TransformImporter::Init();
	RegisterTypeImporter(ObjectType::TRANSFORM, TransformImporter::Import);

	CameraImporter::Init();
	RegisterTypeImporter(ObjectType::CAMERA, CameraImporter::Import);

	MeshRendererImporter::Init();
	RegisterTypeImporter(ObjectType::MESH_RENDERER, MeshRendererImporter::Import);

	CSharpScriptComponentImporter::Init();
	RegisterTypeImporter(ObjectType::CSHARP_SCRIPT_COMPONENT, CSharpScriptComponentImporter::Import);

	AudioSourceImporter::Init();
	RegisterTypeImporter(ObjectType::AUDIO_SOURCE, AudioSourceImporter::Import);
}

ZObject* Factory::CreateInstance(std::string name, ObjectType type)
{
	auto constructor = _typeConstructors.find(type);
	if (constructor != _typeConstructors.end())
	{
		return constructor->second(name, type);
	}

	return nullptr;
}

ZObject* Factory::Copy(std::string name, ZObject* object)
{
	auto copyFunc = _copyFunctions.find(object->GetType());

	if (copyFunc != _copyFunctions.end())
	{
		return copyFunc->second(name, object);
	}

	std::cout << "Factory::Copy: No copy function found for type " << object->GetType() << std::endl;
	return nullptr;
}

void Factory::RegisterType(ObjectType type, ConstructorFunc constructor)
{
	_typeConstructors[type] = constructor;
}

void Factory::RegisterCopyType(ObjectType type, CopyFunc copyFunc)
{
	_copyFunctions[type] = copyFunc;
}

void Factory::RegisterTypeImporter(ObjectType type, ImporterFunc importer)
{
	_importers[type] = importer;
}