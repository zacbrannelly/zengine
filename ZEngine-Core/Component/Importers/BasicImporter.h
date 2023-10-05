#pragma once

#include "ComponentImporter.h"

template<typename T>
class BasicImporter : public ComponentImporter<BasicImporter<T>>
{
public:
	ZObject* ImportImpl(std::string name, nlohmann::json& values) override
	{
		return Factory::CreateInstance<ZObject>(name, _type);
	}

	static BasicImporter<T>* Init()
	{
		auto instance = ComponentImporter<BasicImporter<T>>::GetInstance();
		instance->_type = T::GetStaticType();
	}

private:
	ObjectType _type;
};
