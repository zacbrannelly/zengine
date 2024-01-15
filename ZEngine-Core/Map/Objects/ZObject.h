#pragma once

#include <string>
#include <unordered_set>
#include "ObjectType.h"

class ZObject
{
public:
	ZObject(std::string name, ObjectType type);
	virtual ~ZObject();

	void RegisterDerivedType(ObjectType derivedType);
	bool IsDerivedType(ObjectType derivedType) const;
	std::unordered_set<ObjectType> GetDerivedTypes();

	ObjectType GetType() const;
	std::string GetName() const;

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ObjectType GetStaticType();

	template<class T> bool IsType() const
	{
		return IsDerivedType(T::GetStaticType());
	}

	template<class T> const T* As() const
	{
		if (IsType<T>())
			return static_cast<const T*>(this);
		return nullptr;
	}
private:
	std::string _name;
	ObjectType _type;
	std::unordered_set<ObjectType> _derivedTypesSet;
};

