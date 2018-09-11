#pragma once

#include <string>

enum ObjectType
{
	SHADER, MATERIAL, ENTITY, MAP, CAMERA, TRANSFORM, TEST_RENDERER
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

