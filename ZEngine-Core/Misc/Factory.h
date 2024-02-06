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
	
		if (name.is_string() && type.is_string())
		{
			auto nameValue = name.get<std::string>();
			auto typeValue = type.get<std::string>();
			auto importer = _importers.find(StringToObjectType(typeValue));

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

	template <class T>
	static void RegisterType();

	template<class T>
	static void RegisterTypeImporter();

	template<class T>
	static void RegisterCopyType();

private:
	static std::map<ObjectType, ConstructorFunc> _typeConstructors;
	static std::map<ObjectType, CopyFunc> _copyFunctions;
	static std::map<ObjectType, ImporterFunc> _importers;
};

#include "../Component/Importers/JsonImporter.h"

template <class T>
void Factory::RegisterType()
{
	RegisterType(T::GetStaticType(), &T::CreateInstance);
}

template<class T>
void Factory::RegisterTypeImporter()
{
	JsonImporter<T>::Init();
	RegisterTypeImporter(T::GetStaticType(), JsonImporter<T>::Import);
}

template<class T>
void Factory::RegisterCopyType()
{
	RegisterCopyType(T::GetStaticType(), &T::Copy);
}
