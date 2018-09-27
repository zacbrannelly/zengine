#include "AssetManager.h"
#include <iostream>

#include "Objects/TextureAsset.h"
#include "Objects/ShaderAsset.h"

void AssetManager::Init()
{
	//TODO: Register asset constructors here
	_assetConstructors[ObjectType::TEXTURE_ASSET] = TextureAsset::CreateInstance;
	_assetConstructors[ObjectType::SHADER_ASSET] = ShaderAsset::CreateInstance;
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
