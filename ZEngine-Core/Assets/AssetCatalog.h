#pragma once

#include <vector>
#include "../Map/Objects/ZObject.h"

class Asset;

struct CatalogEntry
{
	int id;
	std::string path;
	int type;
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
	bool GetAssetPathFromID(int id, std::string& path, ObjectType& type);
	std::vector<CatalogEntry> GetAssetsByType(ObjectType type);

	const std::vector<CatalogEntry>& GetCatalogList() const;

private:
	std::vector<CatalogEntry> _catalog;
	int _count;
};

