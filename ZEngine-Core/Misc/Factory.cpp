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
#include "../Component/RigidBody3D.h"
#include "../Component/BoxCollider3D.h"
#include "../Component/SphereCollider3D.h"
#include "../Component/CapsuleCollider3D.h"
#include "../Component/PlaneCollider3D.h"

#include "../Scripting/CSharp/CSharpScript.h"

#include "../Component/Importers/BasicImporter.h"
#include "../Component/Importers/TransformImporter.h"
#include "../Component/Importers/CameraImporter.h"
#include "../Component/Importers/MeshRendererImporter.h"
#include "../Component/Importers/CSharpScriptComponentImporter.h"
#include "../Component/Importers/AudioSourceImporter.h"
#include "../Component/Importers/RigidBody3DImporter.h"
#include "../Component/Importers/BoxCollider3DImporter.h"
#include "../Component/Importers/SphereCollider3DImporter.h"
#include "../Component/Importers/CapsuleCollider3DImporter.h"

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
	RegisterType(ObjectType::RIGID_BODY_3D, &RigidBody3D::CreateInstance);
	RegisterType(ObjectType::BOX_COLLIDER_3D, &BoxCollider3D::CreateInstance);
	RegisterType(ObjectType::SPHERE_COLLIDER_3D, &SphereCollider3D::CreateInstance);
	RegisterType(ObjectType::CAPSULE_COLLIDER_3D, &CapsuleCollider3D::CreateInstance);
	RegisterType(ObjectType::PLANE_COLLIDER_3D, &PlaneCollider3D::CreateInstance);

	// Register copy methods (real-time copying)
	RegisterCopyType(ObjectType::MAP, &Map::Copy);
	RegisterCopyType(ObjectType::ENTITY, &Entity::Copy);
	RegisterCopyType(ObjectType::TRANSFORM, &Transform::Copy);
	RegisterCopyType(ObjectType::MESH_RENDERER, &MeshRenderer::Copy);
	RegisterCopyType(ObjectType::CAMERA, &Camera::Copy);
	RegisterCopyType(ObjectType::CSHARP_SCRIPT_COMPONENT, &CSharpScriptComponent::Copy);
	RegisterCopyType(ObjectType::AUDIO_SOURCE, &AudioSource::Copy);
	RegisterCopyType(ObjectType::RIGID_BODY_3D, &RigidBody3D::Copy);
	RegisterCopyType(ObjectType::BOX_COLLIDER_3D, &BoxCollider3D::Copy);
	RegisterCopyType(ObjectType::SPHERE_COLLIDER_3D, &SphereCollider3D::Copy);
	RegisterCopyType(ObjectType::CAPSULE_COLLIDER_3D, &CapsuleCollider3D::Copy);
	RegisterCopyType(ObjectType::PLANE_COLLIDER_3D, &PlaneCollider3D::Copy);

	// Register component importers (from JSON objects)
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

	RigidBody3DImporter::Init();
	RegisterTypeImporter(ObjectType::RIGID_BODY_3D, RigidBody3DImporter::Import);

	BoxCollider3DImporter::Init();
	RegisterTypeImporter(ObjectType::BOX_COLLIDER_3D, BoxCollider3DImporter::Import);

	SphereCollider3DImporter::Init();
	RegisterTypeImporter(ObjectType::SPHERE_COLLIDER_3D, SphereCollider3DImporter::Import);

	CapsuleCollider3DImporter::Init();
	RegisterTypeImporter(ObjectType::CAPSULE_COLLIDER_3D, CapsuleCollider3DImporter::Import);

	BasicImporter<PlaneCollider3D>::Init();
	RegisterTypeImporter(ObjectType::PLANE_COLLIDER_3D, BasicImporter<PlaneCollider3D>::Import);
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