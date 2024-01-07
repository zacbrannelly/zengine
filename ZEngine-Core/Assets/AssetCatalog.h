#pragma once

#include <vector>
#include <uuid.h>
#include "../Map/Objects/ZObject.h"

class Asset;

struct CatalogEntry
{
	uuids::uuid id;
	std::string path;
	ObjectType type;
};

class AssetCatalog
{
public:
	AssetCatalog();
	~AssetCatalog();

	void ClearCatalog();
	bool LoadCatalog(std::string path = "catalog.zengine");
	bool LoadCatalogFromProjectJson(std::string jsonFilePath);
	bool SaveCatalog(std::string path = "catalog.zengine");

	void RegisterAsset(Asset* asset);
	void RegisterAsset(std::string path, ObjectType type);

	void PushEntry(CatalogEntry entry);

	bool HasAsset(std::string assetPath);
	bool GetAssetPathFromID(uuids::uuid id, std::string& path, ObjectType& type);
	uuids::uuid GetAssetIDFromPath(std::string path);
	std::vector<CatalogEntry> GetAssetsByType(ObjectType type);

	const std::vector<CatalogEntry>& GetCatalogList() const;
	const std::string& GetLastCatalogPath() const;
private:
	std::string _lastCatalogPath;
	std::vector<CatalogEntry> _catalog;
};

