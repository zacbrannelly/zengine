#include "AssetManager.h"
#include <iostream>
#include <algorithm>

#include "Objects/TextureAsset.h"
#include "Objects/ShaderAsset.h"
#include "Objects/MaterialAsset.h"
#include "Objects/MapAsset.h"
#include "Objects/ModelAsset.h"
#include "Objects/ScriptAsset.h"
#include "Objects/CSharpScriptAsset.h"
#include "Objects/AudioAsset.h"

void AssetManager::Init()
{
	//TODO: Register asset constructors here
	_assetConstructors[ObjectType::TEXTURE_ASSET] = TextureAsset::CreateInstance;
	_assetConstructors[ObjectType::SHADER_ASSET] = ShaderAsset::CreateInstance;
	_assetConstructors[ObjectType::MATERIAL_ASSET] = MaterialAsset::CreateInstance;
	_assetConstructors[ObjectType::MAP_ASSET] = MapAsset::CreateInstance;
	_assetConstructors[ObjectType::MODEL_ASSET] = ModelAsset::CreateInstance;
	_assetConstructors[ObjectType::SCRIPT_ASSET] = ScriptAsset::CreateInstance;
	_assetConstructors[ObjectType::CSHARP_SCRIPT_ASSET] = CSharpScriptAsset::CreateInstance;
	_assetConstructors[ObjectType::AUDIO_ASSET] = AudioAsset::CreateInstance;
}

void AssetManager::SetCatalog(AssetCatalog* catalog)
{
	_catalog = catalog;
}

AssetCatalog* AssetManager::GetCatalog() const
{
	return _catalog;
}

Asset* AssetManager::LoadAsset(std::string name, std::string path, ObjectType type)
{
	// Check if the asset has a constructor registered
	if (_assetConstructors.find(type) == _assetConstructors.end())
	{
		std::cout << "ASSET_MANAGER: Failed to find an asset for ObjectType: " << type << std::endl;
		return nullptr;
	}

	// Get the function that constructs this type of asset
	auto constructor = _assetConstructors[type];

	if (constructor != nullptr)
	{
		// Load the new asset
		auto newAsset = constructor(name);
		
		if (newAsset->Load(path))
		{
			// Add to the list of assets (so it can be released later)
			_assets.push_back(newAsset);

			return newAsset;
		}
	}

	return nullptr;
}

Asset* AssetManager::FindAsset(std::string name)
{
	auto it = find_if(_assets.begin(), _assets.end(), [&name](auto asset) { return asset->GetName() == name; });
	return it != _assets.end() ? *it : nullptr;
}

Asset* AssetManager::FindAssetFromPath(std::string path)
{
	auto it = find_if(_assets.begin(), _assets.end(), [&path](auto asset) { return asset->GetPath() == path; });
	return it != _assets.end() ? *it : nullptr;
}

const std::vector<Asset*>& AssetManager::GetAssets() const
{
	return _assets;
}

bool AssetManager::ReloadAsset(std::string name)
{
	auto asset = GetAsset<Asset>(name);

	if (asset != nullptr)
	{
		// Release the asset if still loaded
		if (asset->IsLoaded())
		{
			asset->Release();
		}

		// Reload the asset
		return asset->Load(asset->GetPath());
	}

	std::cout << "ASSET_MANAGER: Failed to find asset for reload: " << name << std::endl;
	return false;
}

void AssetManager::ReleaseAsset(std::string name)
{
	auto asset = GetAsset<Asset>(name);

	if (asset != nullptr)
	{
		asset->Release();
	}
}

void AssetManager::ReleaseAll()
{
	for (auto asset : _assets)
	{
		asset->Release();
	}
}

void AssetManager::DeleteAsset(Asset* asset)
{
	auto it = std::find(_assets.begin(), _assets.end(), asset);

	if (it != _assets.end())
		_assets.erase(it);
}

void AssetManager::DeleteAsset(std::string name)
{
	auto asset = GetAsset<Asset>(name);

	if (asset != nullptr)
	{
		DeleteAsset(asset);
	}
}

void AssetManager::DeleteAll()
{
	for (auto asset : _assets)
	{
		DeleteAsset(asset);
	}
}

void AssetManager::Shutdown()
{
	DeleteAll();
}
