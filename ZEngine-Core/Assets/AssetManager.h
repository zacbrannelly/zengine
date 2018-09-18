#include <map>
#include <vector>

#include "../Misc/Singleton.h"
#include "Asset.h"

class AssetManager : public Singleton<AssetManager>
{
public:
	void Init();
	void Shutdown();

	Asset* LoadAsset(std::string name, std::string path, ObjectType type);

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
				return asset;
		}
		
		return nullptr;
	}

private:
	std::map<ObjectType, Asset*(*)(std::string)> _assetConstructors;
	std::vector<Asset*> _assets;
};