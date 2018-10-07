#pragma once

#include <string>

enum ObjectType
{
	SHADER, MATERIAL, ENTITY, MAP, CAMERA, TRANSFORM, MESH, MESH_RENDERER, SCRIPT, SCRIPT_COMPONENT, TEST_RENDERER,
	TEXTURE_ASSET, SHADER_ASSET, MATERIAL_ASSET, MODEL_ASSET, MAP_ASSET
};

class ZObject
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

