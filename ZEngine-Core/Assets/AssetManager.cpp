#include "AssetManager.h"
#include <iostream>
#include <algorithm>

#include "Objects/TextureAsset.h"
#include "Objects/ShaderAsset.h"
#include "Objects/MaterialAsset.h"
#include "Objects/MapAsset.h"
#include "Objects/ModelAsset.h"
#include "Objects/CSharpScriptAsset.h"
#include "Objects/AudioAsset.h"
#include "../Utilities/Directory.h"

void AssetManager::Init()
{
	_basePath = "";
}

void AssetManager::SetCatalog(AssetCatalog* catalog)
{
	_catalog = catalog;
}

AssetCatalog* AssetManager::GetCatalog() const
{
	return _catalog;
}

void AssetManager::SetBasePath(std::string basePath)
{
    _basePath = basePath;
}

std::string AssetManager::GetBasePath() const
{
    return _basePath;
}

Asset* AssetManager::CreateAsset(std::string name, std::string path, ObjectType type)
{
	auto newAsset = Factory::CreateDefaultInstance<Asset>(name, type);
	newAsset->SetPath(path);

	// Add to the list of assets (so it can be released later)
	_assets.push_back(newAsset);

	// TODO: Save the new asset to file
	// TODO: Add the new asset to the catalog

	return newAsset;
}

Asset* AssetManager::LoadAsset(std::string path, ObjectType type)
{
	// Derive name from the filename without extension
	auto name = Directory::GetFilename(path);
	name = name.substr(0, name.find_last_of('.'));

	return LoadAsset(name, path, type);
}

Asset* AssetManager::LoadAsset(std::string name, std::string path, ObjectType type)
{
	// Create a new empty instance.
	auto newAsset = Factory::CreateInstance<Asset>(name, type);
	
	// Prepend base path to the provided path.
	if (_basePath != "" && path[0] != '/')
		path = _basePath + "/" + path;
	
	// Load the asset from the provided path.
	if (newAsset->Load(path))
	{
		// Add to the list of assets (so it can be released later)
		_assets.push_back(newAsset);

		return newAsset;
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
	
	delete asset;
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
		delete asset;
	}
	_assets.clear();
}

void AssetManager::Shutdown()
{
	DeleteAll();
}
