#include <map>
#include <vector>

#include "../Misc/Singleton.h"
#include "Asset.h"

class AssetCatalog;

class AssetManager : public Singleton<AssetManager>
{
public:
	void Init();
	void Shutdown();

	void SetCatalog(AssetCatalog* catalog);
	AssetCatalog* GetCatalog() const;

	Asset* LoadAsset(std::string name, std::string path, ObjectType type);

	Asset* FindAsset(std::string name);
	Asset* FindAssetFromPath(std::string path);

	bool ReloadAsset(std::string name);
	void ReleaseAsset(std::string name);
	void ReleaseAll();

	void DeleteAsset(Asset* asset);
	void DeleteAsset(std::string name);
	void DeleteAll();

	template<typename T = class Asset>
	T* GetAsset(std::string name) const
	{
		for (auto asset : _assets)
		{
			if (asset->GetName() == name)
				return static_cast<T*>(asset);
		}
		
		return nullptr;
	}

private:
	std::map<ObjectType, Asset*(*)(std::string)> _assetConstructors;
	std::vector<Asset*> _assets;
	AssetCatalog* _catalog;
};