#include "MaterialAsset.h"
#include "ShaderAsset.h"
#include "TextureAsset.h"

#include "../AssetManager.h"
#include "../AssetCatalog.h"

#include "../../Rendering/Material.h"
#include "../../Utilities/Directory.h"
#include "../../Utilities/JsonHelpers.h"

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
		auto textureArray = it->get<json::array_t>();
		ReadTextures(textureArray, _material);
	}

	// Load uniforms
	it = root.find("uniforms");
	if (it != root.end())
	{
		auto uniformsArray = it->get<json::array_t>();
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
			auto id = it->get<uuids::uuid>();
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
				path = assetDir + it->get<string>();
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

		auto name = uniformObj.at("name").get<string>();
		auto type = uniformObj.at("type").get<bgfx::UniformType::Enum>();
		auto numElements = uniformObj.at("numElements").get<unsigned short>();
		auto values = uniformObj.at("values");

		// Register the uniform variable.
		material->RegisterUniform(name, type, numElements);

		// Store the uniform data in the heap in a format that the material can use.
		void* data = nullptr;
		switch (type)
		{
			case bgfx::UniformType::Vec4:
			{
				std::vector<glm::vec4>* vec4 = new std::vector<glm::vec4>(numElements);
				from_json(values, *vec4);
				data = vec4->data();
				break;
			}
			case bgfx::UniformType::Mat3:
			{
				std::vector<glm::mat3>* mat3 = new std::vector<glm::mat3>(numElements);
				from_json(values, *mat3);
				data = mat3->data();
				break;
			}
			case bgfx::UniformType::Mat4:
			{
				std::vector<glm::mat4>* mat4 = new std::vector<glm::mat4>(numElements);
				from_json(values, *mat4);
				data = mat4->data();
				break;
			}
		}

		// Pass the uniform data to the material
		if (data != nullptr) material->SetUniform(name, data, numElements);
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
	{
		path = identifier.get<string>();
	}
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
