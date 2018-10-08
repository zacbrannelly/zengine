#pragma once

#include <map>
#include <string>
#include "../Map/Objects/ZObject.h"
#include "../../json/json.hpp"

typedef ZObject*(*ConstructorFunc)(std::string, ObjectType);
typedef ZObject*(*ImporterFunc)(std::string, nlohmann::json::object_t&);

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

	template<class T>
	static T* ImportInstance(nlohmann::json::object_t& values)
	{
		auto name = values.at("name");
		auto type = values.at("type");
	
		if (name.is_string() && type.is_number_unsigned())
		{
			auto nameValue = name.get<std::string>();
			auto typeValue = type.get<unsigned int>();
			auto importer = _importers.find((ObjectType)typeValue);

			if (importer != _importers.end())
			{
				return static_cast<T*>(importer->second(nameValue, values));
			}
		}

		return nullptr;
	}

	static void RegisterType(ObjectType type, ConstructorFunc constructor);
	static void RegisterTypeImporter(ObjectType type, ImporterFunc importerFunc);

private:
	static std::map<ObjectType, ConstructorFunc> _typeConstructors;
	static std::map<ObjectType, ImporterFunc> _importers;
};

