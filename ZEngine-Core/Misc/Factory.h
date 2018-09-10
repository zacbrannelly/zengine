#pragma once

#include <map>
#include <string>
#include "../Map/Objects/ZObject.h"

typedef ZObject*(*ConstructorFunc)(std::string, ObjectType);

class Factory
{
public:
	static void Init();

	template<class T> 
	static T* CreateInstance(std::string name, ObjectType type)
	{
		auto constructor = _typeConstructors.find(type);

		if (constructor != _typeConstructors.end())
		{
			return static_cast<T*>(constructor->second(name, type));
		}

		return nullptr;
	}

	static void RegisterType(ObjectType type, ConstructorFunc constructor);

private:
	static std::map<ObjectType, ConstructorFunc> _typeConstructors;
};

