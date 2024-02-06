#pragma once

#include "ComponentImporter.h"

template<typename T>
class JsonImporter : public ComponentImporter<JsonImporter<T>>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override
	{
    // Create an instance of the object.
		auto instance =  Factory::CreateInstance<T>(name, _type);
    
    // Inject the values from the json into the instance.
    // This is done via functions defined by the JSON_SCHEMA_BEGIN/JSON_SCHEMA_END macros in
    // the corresponding header file for the class.
    // See Utilities/JsonHelpers.h for more information.
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
