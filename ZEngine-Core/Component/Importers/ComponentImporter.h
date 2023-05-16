#pragma once

#include "../../Misc/Singleton.h"
#include <nlohmann/json.hpp>
#include "../../Map/Objects/ZObject.h"
#include <glm/glm.hpp>

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

	// Read helper functions
	std::string ReadString(std::string key, nlohmann::json& values) const;
	int ReadInt(std::string key, nlohmann::json& values) const;
	unsigned int ReadUnsignedInt(std::string key, nlohmann::json& values) const;
	float ReadFloat(std::string key, nlohmann::json& values) const;
	double ReadDouble(std::string key, nlohmann::json& values) const;
	bool ReadBool(std::string key, nlohmann::json& values) const;

	// Array read helper function
	template<typename T2, std::size_t S>
	std::array<T2, S> ReadArray(std::string key, nlohmann::json& values) const;

	// Dynamic array read helper function
	template<typename T2>
	std::vector<T2> ReadDynamicArray(std::string key, nlohmann::json& values) const;
	
	// Vector read helper function
	template<int S>
	glm::vec<S, float, glm::highp> ReadVector(std::string key, nlohmann::json& values) const;

	// General Helper functions
	bool HasKey(std::string key, nlohmann::json& values) const;
};

// Implementation of the importer class functions
template<typename T>
bool ComponentImporter<T>::HasKey(std::string key, nlohmann::json& values) const
{
	return values.find(key) != values.end();
}

template<typename T>
std::string ComponentImporter<T>::ReadString(std::string key, nlohmann::json& values) const
{
	auto it = values.find(key);
	if (it != values.end() && it.value().is_string())
	{
		return values.at(key).get<std::string>();
	}

	return "";
}

template<typename T>
int ComponentImporter<T>::ReadInt(std::string key, nlohmann::json& values) const
{
	auto it = values.find(key);
	if (it != values.end() && it.value().is_number_integer())
	{
		return values.at(key).get<int>();
	}

	return -1;
}

template<typename T>
unsigned int ComponentImporter<T>::ReadUnsignedInt(std::string key, nlohmann::json& values) const
{
	auto it = values.find(key);
	if (it != values.end() && it.value().is_number_unsigned())
	{
		return values.at(key).get<unsigned int>();
	}
	
	return 0;
}

template<typename T>
float ComponentImporter<T>::ReadFloat(std::string key, nlohmann::json& values) const
{
	auto it = values.find(key);
	if (it != values.end() && it.value().is_number_float())
	{
		return it.value().get<float>();
	}

	return -1.0f;
}

template<typename T>
double ComponentImporter<T>::ReadDouble(std::string key, nlohmann::json& values) const
{
	auto it = values.find(key);
	if (it != values.end() && it.value().is_number())
	{
		return it.value().get<double>();
	}

	return -1.0;
}

template<typename T>
bool ComponentImporter<T>::ReadBool(std::string key, nlohmann::json& values) const
{
	auto it = values.find(key);
	if (it != values.end() && it.value().is_boolean())
	{
		return it.value().get<bool>();
	}

	return false;
}

template<typename T>
template<typename T2, std::size_t S>
std::array<T2, S> ComponentImporter<T>::ReadArray(std::string key, nlohmann::json& values) const
{
	std::array<T2, S> result;

	auto it = values.find(key);

	if (it != values.end() && it.value().is_array())
	{
		auto arr = it.value().get<nlohmann::json::array_t>();

		if (arr.size() == S)
		{
			for (int i = 0; i < S; i++)
			{
				result[i] = arr[i].get<T2>();
			}
		}
	}

	return result;
}

template<typename T>
template<typename T2>
std::vector<T2> ComponentImporter<T>::ReadDynamicArray(std::string key, nlohmann::json& values) const
{
	std::vector<T2> result;

	auto it = values.find(key);

	if (it != values.end() && it.value().is_array())
	{
		for (auto value : it.value().get<nlohmann::json::array_t>())
		{
			result.push_back(value.get<T2>());
		}
	}

	return result;
}

template<typename T>
template<int S>
glm::vec<S, float, glm::highp> ComponentImporter<T>::ReadVector(std::string key, nlohmann::json& values) const
{
	auto result = glm::vec<S, float, glm::highp>();
	auto data = ReadArray<float, S>(key, values);

	// Copy the data across the the vector
	memcpy(&result, &data, sizeof(float) * S);

	return result;
}
