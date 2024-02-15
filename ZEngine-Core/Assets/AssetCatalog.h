#pragma once

#include <vector>
#include <uuid.h>
#include "../Map/Objects/ZObject.h"
#include "../Utilities/File.h"

class Asset;

struct CatalogEntry
{
	uuids::uuid id;
	std::string path;
	ObjectType type;
	std::string name;
};

class AssetCatalog
{
public:
	AssetCatalog();
	~AssetCatalog();

	void ClearCatalog();
	bool LoadCatalog(std::string path = "catalog.zengine");
	bool LoadCatalogFromProjectJson(std::string jsonFilePath);
	bool LoadCatalogFromProjectJson(const nlohmann::json& json, std::string basePath = "");

	bool SaveCatalog(std::string path = "catalog.zengine");
	bool SaveCatalogToProjectJson(nlohmann::json& json, std::string basePath = "");

	void RegisterAsset(Asset* asset);
	void RegisterAsset(std::string path, ObjectType type);

	void PushEntry(CatalogEntry entry);

	bool HasAsset(std::string assetPath);
	bool GetAssetPathFromID(uuids::uuid id, std::string& path, ObjectType& type);
	uuids::uuid GetAssetIDFromPath(std::string path);
	std::vector<CatalogEntry> GetAssetsByType(ObjectType type);

	const std::vector<CatalogEntry>& GetCatalogList() const;
	const std::string& GetLastCatalogPath() const;

	std::string ResolveAssetNameFromPath(const std::string& path);
private:
	std::string _lastCatalogPath;
	std::vector<CatalogEntry> _catalog;
};

