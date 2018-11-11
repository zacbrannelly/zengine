#include "ZObject.h"

ZObject::ZObject(std::string name, ObjectType type): _name(name), _type(type), IScriptable(type)
{
}

std::string ZObject::GetName() const
{
	return _name;
}

ObjectType ZObject::GetType() const
{
	return _type;
}

ZObject::~ZObject()
{
}
