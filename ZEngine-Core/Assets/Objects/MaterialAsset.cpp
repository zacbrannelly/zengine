#include "MaterialAsset.h"
#include "ShaderAsset.h"
#include "TextureAsset.h"

#include "../AssetManager.h"
#include "../AssetCatalog.h"
#include "../../Rendering/StandardShaders.h"
#include "../../Rendering/Material.h"
#include "../../Utilities/Directory.h"
#include "../../Utilities/JsonHelpers.h"
#include "../../Logging/LoggingSystem.h"
#include "../../Rendering/Lighting/LightingSystem.h"

#include <fstream>
#include <string>

using namespace std;
using namespace nlohmann;
using namespace ZEngine;

static const string MODULE_NAME = "MaterialAsset";

MaterialAsset::MaterialAsset(string name) : Asset(name, ObjectType::MATERIAL_ASSET)
{
	RegisterDerivedType(MATERIAL_ASSET);
	_material = nullptr;
}

ZObject* MaterialAsset::CreateInstance(string name, ObjectType type)
{
	return new MaterialAsset(name);
}

ZObject* MaterialAsset::CreateDefaultInstance(string name, ObjectType type)
{
	auto newInstance = new MaterialAsset(name);
	newInstance->_material = Factory::CreateDefaultInstance<Material>(name);
	return newInstance;
}

bool MaterialAsset::Load(string path)
{
	SetPath(path);
	ifstream in(path, ios::in);

	if (!in.is_open())
	{
		LoggingSystem::GetInstance()->LogError("Load: Failed to open file: " + path, MODULE_NAME);
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
		LoggingSystem::GetInstance()->LogError("Load: Invalid material, no name was found!", MODULE_NAME);
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

	// Read uniforms from the shader.
	PopulateUniformsFromShader();

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
				LoggingSystem::GetInstance()->LogError("ReadTextures: Failed to find texture for sampler: " + samplerName.get<string>(), MODULE_NAME);
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
				LoggingSystem::GetInstance()->LogError("ReadTextures: Failed to find texture for sampler: " + samplerName.get<string>(), MODULE_NAME);
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

void MaterialAsset::ReadShader(json& shaderRef, Material* material)
{
	auto assetManager = AssetManager::GetInstance();
	auto catalog = assetManager->GetCatalog();

	string path = "";
	ObjectType type;

	if (shaderRef.find("assetId") != shaderRef.end())
	{
		// Resolve path via Asset UUID.
		const auto maybeId = uuids::uuid::from_string(shaderRef.at("assetId").get<string>());
		if (!maybeId.has_value())
		{
			LoggingSystem::GetInstance()->LogError("ReadShader: Invalid shader asset ID: " + shaderRef.at("assetId").get<string>(), MODULE_NAME);
			return;
		}

		if (!catalog->GetAssetPathFromID(maybeId.value(), path, type))
		{
			LoggingSystem::GetInstance()->LogError("ReadShader: Failed to find shader for material: " + _material->GetName(), MODULE_NAME);
			return;
		}
	}
	else if (shaderRef.find("path") != shaderRef.end())
	{
		// Use path provided in the JSON.
		path = shaderRef.at("path").get<string>();
	}
	else if (shaderRef.find("standardShader") != shaderRef.end())
	{
		// Use a standard shader.
		auto shader = StandardShaders::GetShader(shaderRef.at("standardShader").get<string>());
		if (shader == nullptr) {
			LoggingSystem::GetInstance()->LogError("ReadShader: Failed to find standard shader for material: " + _material->GetName(), MODULE_NAME);
			return;
		}
		material->SetShader(shader);
		return;
	}
	else
	{
		LoggingSystem::GetInstance()->LogError("ReadShader: Failed to find shader for material: " + _material->GetName(), MODULE_NAME);
		return;
	}

	// Load the shader asset from the resolved path.
	auto asset = assetManager->FindAssetFromPath(path);
	if (asset == nullptr)
	{
		asset = assetManager->LoadAsset(path, path, SHADER_ASSET);
	}

	// Assign the shader to the material.
	if (asset != nullptr)
	{
		auto shaderAsset = asset->Cast<ShaderAsset>();
		material->SetShader(shaderAsset->GetShader());
	}
	else
	{
		LoggingSystem::GetInstance()->LogError("ReadShader: Failed to load shader for material: " + _material->GetName(), MODULE_NAME);
	}
}

void MaterialAsset::PopulateUniformsFromShader()
{
	if (_material == nullptr) return;
	if (_material->GetShader() == nullptr) return;

	auto shader = _material->GetShader();
	auto uniforms = shader->GetUniforms();
	auto lightingSystem = LightingSystem::GetInstance();

	auto existingUniforms = _material->GetUniforms();
	auto existingSamplers = _material->GetSamplers();

	for (auto& uniform : uniforms)
	{
		// Don't register uniforms managed by the lighting system.
		if (lightingSystem->IsLightingUniform(uniform.handle)) continue;

		if (uniform.type == bgfx::UniformType::Sampler)
		{
			auto maybeSampler = existingSamplers.find(uniform.name);
			if (maybeSampler == existingSamplers.end())
			{
				_material->RegisterSampler(uniform.name);
			}
		}
		else
		{
			auto maybeUniform = existingUniforms.find(uniform.name);
			if (maybeUniform == existingUniforms.end())
			{
				_material->RegisterUniform(uniform.name, uniform.type, uniform.numElements);
			}
		}
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
