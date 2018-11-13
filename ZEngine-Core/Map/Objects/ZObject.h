#pragma once

#include <string>
#include "../../Scripting/IScriptable.h"

enum ObjectType
{
	// WARNING: THE ORDER OF THESE MUST NOT BE CHANGED! FOR MAP LOADING!
	SHADER, MATERIAL, ENTITY, MAP, CAMERA, TRANSFORM, MESH, MESH_RENDERER, SCRIPT, SCRIPT_COMPONENT, TEST_RENDERER,
	TEXTURE_ASSET, SHADER_ASSET, MATERIAL_ASSET, MODEL_ASSET, MAP_ASSET, SCRIPT_ASSET, AUDIO_ASSET, AUDIO_SOURCE
};

class ZObject : public IScriptable
{
public:
	ZObject(std::string name, ObjectType type);
	virtual ~ZObject();

	ObjectType GetType() const;
	std::string GetName() const;

	static ZObject* CreateInstance(std::string name, ObjectType type);
private:
	std::string _name;
	ObjectType _type;
};

