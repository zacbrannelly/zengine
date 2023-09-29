#include "ZObject.h"

ZObject::ZObject(std::string name, ObjectType type): _name(name), _type(type)
{
}

void ZObject::RegisterDerivedType(ObjectType derivedType)
{
	_derivedTypesSet.insert(derivedType);
}

bool ZObject::IsDerivedType(ObjectType derivedType) const
{
	return _derivedTypesSet.find(derivedType) != _derivedTypesSet.end();
}

std::unordered_set<ObjectType> ZObject::GetDerivedTypes()
{
	return _derivedTypesSet;
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
