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
#include "../Component/MeshCollider3D.h"
#include "../Scripting/CSharp/CSharpScript.h"

#include <iostream>

std::map<ObjectType, ConstructorFunc> Factory::_typeConstructors;
std::map<ObjectType, CopyFunc> Factory::_copyFunctions;
std::map<ObjectType, ImporterFunc> Factory::_importers;

void Factory::Init()
{
	// Register factory methods (real-time creation)
	RegisterType<Entity>();
	RegisterType<Map>();
	RegisterType<Material>();
	RegisterType<Mesh>();
	RegisterType<Shader>();
	RegisterType<Camera>();
	RegisterType<Transform>();
	RegisterType<MeshRenderer>();
	RegisterType<CSharpScript>();
	RegisterType<CSharpScriptComponent>();
	RegisterType<AudioSource>();
	RegisterType<TestRenderer>();
	RegisterType<RigidBody3D>();
	RegisterType<BoxCollider3D>();
	RegisterType<SphereCollider3D>();
	RegisterType<CapsuleCollider3D>();
	RegisterType<PlaneCollider3D>();
	RegisterType<MeshCollider3D>();

	// Register copy methods (real-time copying)
	RegisterCopyType<Map>();
	RegisterCopyType<Entity>();
	RegisterCopyType<Transform>();
	RegisterCopyType<MeshRenderer>();
	RegisterCopyType<Camera>();
	RegisterCopyType<CSharpScriptComponent>();
	RegisterCopyType<AudioSource>();
	RegisterCopyType<RigidBody3D>();
	RegisterCopyType<BoxCollider3D>();
	RegisterCopyType<SphereCollider3D>();
	RegisterCopyType<CapsuleCollider3D>();
	RegisterCopyType<PlaneCollider3D>();
	RegisterCopyType<MeshCollider3D>();

	// Register component importers (from JSON objects)
	RegisterTypeImporter<Transform>();
	RegisterTypeImporter<Camera>();
	RegisterTypeImporter<MeshRenderer>();
	RegisterTypeImporter<CSharpScriptComponent>();
	RegisterTypeImporter<AudioSource>();
	RegisterTypeImporter<RigidBody3D>();
	RegisterTypeImporter<BoxCollider3D>();
	RegisterTypeImporter<SphereCollider3D>();
	RegisterTypeImporter<CapsuleCollider3D>();
	RegisterTypeImporter<PlaneCollider3D>();
	RegisterTypeImporter<MeshCollider3D>();
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