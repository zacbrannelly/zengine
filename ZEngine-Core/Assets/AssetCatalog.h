#pragma once

#include <vector>
#include "../Map/Objects/ZObject.h"

class Asset;

struct CatalogEntry
{
	int id;
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
	bool SaveCatalog(std::string path = "catalog.zengine");

	void RegisterAsset(Asset* asset);
	void RegisterAsset(std::string path, ObjectType type);

	bool HasAsset(std::string assetPath);
	bool GetAssetPathFromID(int id, std::string& path, ObjectType& type);
	int GetAssetIDFromPath(std::string path);
	std::vector<CatalogEntry> GetAssetsByType(ObjectType type);

	const std::vector<CatalogEntry>& GetCatalogList() const;
	const std::string& GetLastCatalogPath() const;
private:
	std::string _lastCatalogPath;
	std::vector<CatalogEntry> _catalog;
	int _count;
};

