#pragma once

#include <map>
#include <string>
#include "../Map/Objects/ZObject.h"
#include <nlohmann/json.hpp>

typedef ZObject*(*ConstructorFunc)(std::string, ObjectType);
typedef ZObject*(*CopyFunc)(std::string, ZObject*);
typedef ZObject*(*ImporterFunc)(std::string, nlohmann::json&);

class Factory
{
public:
	static void Init();

	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* Copy(std::string name, ZObject* object);

	template<class T> 
	static T* CreateInstance(std::string name, ObjectType type)
	{
		return static_cast<T*>(CreateInstance(name, type));
	}

	template<class T>
	static T* Copy(std::string name, ZObject* object)
	{
		return static_cast<T*>(Copy(name, object));
	}

	template<class T>
	static T* ImportInstance(nlohmann::json& values)
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
	static void RegisterCopyType(ObjectType type, CopyFunc copyFunc);
	static void RegisterTypeImporter(ObjectType type, ImporterFunc importerFunc);
private:
	static std::map<ObjectType, ConstructorFunc> _typeConstructors;
	static std::map<ObjectType, CopyFunc> _copyFunctions;
	static std::map<ObjectType, ImporterFunc> _importers;
};

