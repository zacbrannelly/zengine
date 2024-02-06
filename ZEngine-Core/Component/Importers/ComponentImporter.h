#pragma once

#include "../../Misc/Singleton.h"
#include <nlohmann/json.hpp>

template<typename T>
class ComponentImporter : public Singleton<T>
{
public:
	static ZObject* Import(std::string name, nlohmann::json& values)
	{
		return T::GetInstance()->ImportImpl(name, values);
	}

	static void Init()
	{
		T::GetInstance();
	}

protected:
	virtual ZObject* ImportImpl(std::string name, nlohmann::json& values) = 0;
};
