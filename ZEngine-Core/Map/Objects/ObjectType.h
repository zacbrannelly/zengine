#pragma once

#include <string>

enum ObjectType
{
	// NOTE: Make sure to update ObjectType.cpp when adding new types!
	SHADER,
	MATERIAL,
	ENTITY,
	MAP,
	CAMERA,
	TRANSFORM,
	MESH,
	MESH_RENDERER,
	TEST_RENDERER,
	TEXTURE_ASSET,
	SHADER_ASSET,
	MATERIAL_ASSET,
	MODEL_ASSET,
	MAP_ASSET,
	AUDIO_ASSET,
	AUDIO_SOURCE,
	CSHARP_SCRIPT_COMPONENT,
	CSHARP_SCRIPT,
	CSHARP_SCRIPT_ASSET,
};

std::string ObjectTypeToString(ObjectType type);
ObjectType StringToObjectType(std::string type);
