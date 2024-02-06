#pragma once

#include "ComponentImporter.h"

template<typename T>
class JsonImporter : public ComponentImporter<JsonImporter<T>>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override
	{
		auto instance =  Factory::CreateInstance<T>(name, _type);
    from_json(values, *instance);

    return instance;
	}

	static void Init()
	{
		auto instance = ComponentImporter<JsonImporter<T>>::GetInstance();
		instance->_type = T::GetStaticType();
	}

private:
	ObjectType _type;
};
