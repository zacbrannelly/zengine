#include "MaterialAsset.h"
#include "ShaderAsset.h"
#include "TextureAsset.h"

#include "../AssetManager.h"
#include "../AssetCatalog.h"

#include <nlohmann/json.hpp>
#include <uuid.h>
#include "../../Rendering/Material.h"
#include "../../Utilities/Directory.h"

#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace nlohmann;

MaterialAsset::MaterialAsset(string name) : Asset(name, ObjectType::MATERIAL_ASSET)
{
	RegisterDerivedType(MATERIAL_ASSET);
	_material = nullptr;
}

Asset* MaterialAsset::CreateInstance(string name)
{
	return new MaterialAsset(name);
}

bool MaterialAsset::Load(string path)
{
	SetPath(path);
	ifstream in(path, ios::in);

	if (!in.is_open())
	{
		cout << "MATERIAL_ASSET: Failed to open file: " << path << endl;
		return false;
	}

	// Import JSON file 
	json root;
	in >> root;

	auto assetManager = AssetManager::GetInstance();

	// Extract the name from the json object and instantiate a new material 
	auto it = root.find("name");
	if (it != root.end() && (*it).is_string() && _material == nullptr)
	{
		_material = new Material((*it).get<string>());
	}
	else if (_material != nullptr)
	{
		_material->Release();
	}

	if (_material == nullptr)
	{
		cout << "MATERIAL_ASSET: Invalid material, no name was found!" << endl;
		return false;
	}

	// Load textures
	it = root.find("textures");
	if (it != root.end())
	{
		auto textureArray = (*it).get<json::array_t>();
		ReadTextures(textureArray, _material);
	}

	// Load uniforms
	it = root.find("uniforms");
	if (it != root.end())
	{
		auto uniformsArray = (*it).get<json::array_t>();
		ReadUniforms(uniformsArray, _material);
	}

	// Load shader
	it = root.find("shader");
	if (it != root.end())
	{
		ReadShader(*it, _material);
	}

	return true;
}

void MaterialAsset::ReadTextures(json::array_t& values, Material* material)
{
	auto assetManager = AssetManager::GetInstance();

	// Load all of the samplers and set them (if possible)
	for (auto& item : values)
	{
		if (!item.is_object()) continue;

		auto textureObj = item.get<json>();
		auto samplerName = textureObj.at("name");
		
		string path;

		auto it = textureObj.find("id");
		if (it != textureObj.end())
		{
			ObjectType type;
			uuids::uuid id = uuids::uuid::from_string(it.value().get<string>()).value();
			if (!assetManager->GetCatalog()->GetAssetPathFromID(id, path, type))
			{
				cout << "MATERIAL_ASSET: Failed to find texture for sampler: " << samplerName.get<string>() << endl;
				continue;
			}
		}
		else
		{
			it = textureObj.find("path");

			auto assetDir = Directory::GetBasePath(GetPath());
			if (assetDir[assetDir.length() - 1] != '/')
				assetDir += '/';
			
			if (it != textureObj.end())
				path = assetDir + it.value().get<string>();
			else
			{
				cout << "MATERIAL_ASSET: Failed to find texture for sampler: " << samplerName.get<string>() << endl;
				continue;
			}
		}


		if (samplerName.is_string())
		{
			material->RegisterSampler(samplerName.get<string>());

			// Check whether the texture is already loaded
			auto textureAsset = assetManager->FindAssetFromPath(path);

			if (textureAsset == nullptr)
			{
				textureAsset = assetManager->LoadAsset(samplerName.get<string>(), path, TEXTURE_ASSET);
				textureAsset->Cast<TextureAsset>()->LoadTexture();
			}

			if (textureAsset != nullptr)
			{
				material->SetTexture(samplerName.get<string>(), textureAsset->Cast<TextureAsset>()->GetTexture()->GetHandle());
			}
		}
	}
}

void MaterialAsset::ReadUniforms(json::array_t& values, Material* material)
{
	for (auto& uniform : values)
	{
		auto uniformObj = uniform.get<json>();

		auto name = uniformObj.at("name");
		auto type = uniformObj.at("type");
		auto numElements = uniformObj.at("numElements");
		auto values = uniformObj.at("values");

		// Register the uniforms
		if (name.is_string() && type.is_number_unsigned() && numElements.is_number_unsigned())
		{
			material->RegisterUniform(name.get<string>(), (bgfx::UniformType::Enum)type.get<unsigned int>(), numElements.get<unsigned short>());
		}

		if (values.is_array())
		{
			auto valuesArray = values.get<json::array_t>();
			auto uniformType = (bgfx::UniformType::Enum)type.get<unsigned int>();
			auto num = numElements.get<int>();

			void* data = nullptr;

			switch (uniformType)
			{
			case bgfx::UniformType::Vec4:
				data = ReadVec4From(valuesArray, num)->data();
				break;

			case bgfx::UniformType::Mat3:
				data = ReadMat3From(valuesArray, num)->data();
				break;

			case bgfx::UniformType::Mat4:
				data = ReadMat4From(valuesArray, num)->data();
				break;
			}

			if (data != nullptr)
				material->SetUniform(name.get<string>(), data, numElements.get<unsigned short>());
		}
	}
}

void MaterialAsset::ReadShader(json& identifier, Material* material)
{
	auto assetManager = AssetManager::GetInstance();
	auto catalog = assetManager->GetCatalog();

	string path = "";
	ObjectType type;

	const auto maybeId = uuids::uuid::from_string(identifier.get<string>());
	if (!maybeId.has_value())
		path = identifier.get<string>();
	else if (catalog != nullptr)
	{
		if (!catalog->GetAssetPathFromID(maybeId.value(), path, type))
		{
			cout << "Failed to find the shader for material: " << _material->GetName() << endl;
			return;
		}
	}

	auto asset = assetManager->FindAssetFromPath(path);

	if (asset == nullptr)
	{
		asset = assetManager->LoadAsset(path, path, SHADER_ASSET);
	}

	if (asset != nullptr)
	{
		auto shaderAsset = asset->Cast<ShaderAsset>();
		material->SetShader(shaderAsset->GetShader());
	}
}

std::vector<glm::vec4>* MaterialAsset::ReadVec4From(json::array_t& values, int num)
{
	std::vector<glm::vec4>* results = new std::vector<glm::vec4>();

	if (all_of(values.begin(), values.end(), [&values](auto value) { return value.is_number(); }))
	{
		// Extract vec4 from the values array 'num' times
		int index = 0;
		for (int i = 0; i < num; ++i)
		{
			results->push_back(glm::vec4(values[index].get<float>(), values[index + 1].get<float>(), values[index + 2].get<float>(), values[index + 3].get<float>()));
			index += 4;
		}
	}

	return results;
}

std::vector<glm::mat3>* MaterialAsset::ReadMat3From(json::array_t& values, int num)
{
	std::vector<glm::mat3>* results = new std::vector<glm::mat3>(num);

	if (all_of(values.begin(), values.end(), [&values](auto value) { return value.is_number(); }))
	{
		std::vector<float> buffer;

		// Add all the data into a buffer
		for (int i = 0; i < num * 3 * 3; i++)
		{
			buffer.push_back(values[i].get<float>());
		}

		// Copy the data from the buffer into the results
		for (int i = 0; i < num; i++)
		{
			memcpy(results->data(), buffer.data() + i * 3 * 3, sizeof(float) * 3 * 3);
		}
	}

	return results;
}

std::vector<glm::mat4>* MaterialAsset::ReadMat4From(json::array_t& values, int num)
{
	std::vector<glm::mat4>* results = new std::vector<glm::mat4>(num);

	if (all_of(values.begin(), values.end(), [&values](auto value) { return value.is_number(); }))
	{
		std::vector<float> buffer;

		// Add all the data into a buffer
		for (int i = 0; i < num * 4 * 4; i++)
		{
			buffer.push_back(values[i].get<float>());
		}

		// Copy the data from the buffer into the results
		for (int i = 0; i < num; i++)
		{
			memcpy(results->data(), buffer.data() + i * 4 * 4, sizeof(float) * 4 * 4);
		}
	}

	return results;
}

Material* MaterialAsset::GetMaterial() const
{
	return _material;
}

void MaterialAsset::Release()
{
	if (_material != nullptr)
		delete _material;
}

MaterialAsset::~MaterialAsset()
{
}
