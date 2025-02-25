#pragma once

#include <map>
#include <vector>

#include "../Misc/Singleton.h"
#include "Asset.h"

namespace ZEngine
{
	class AssetCatalog;

	class AssetManager : public Singleton<AssetManager>
	{
	public:
		void Init();
		void Shutdown();

		void SetCatalog(AssetCatalog* catalog);
		AssetCatalog* GetCatalog() const;

		void SetBasePath(std::string basePath);
		std::string GetBasePath() const;

		Asset* CreateAsset(std::string path, ObjectType type);
		Asset* CreateAsset(std::string name, std::string path, ObjectType type);

		template<typename T = class Asset>
		T* CreateAsset(std::string path)
		{
			return static_cast<T*>(CreateAsset(path, T::GetStaticType()));
		}

		template<typename T = class Asset>
		T* CreateAsset(std::string name, std::string path)
		{
			return static_cast<T*>(CreateAsset(name, path, T::GetStaticType()));
		}

		Asset* LoadAsset(std::string path, ObjectType type);
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

		const std::vector<Asset*>& GetAssets() const;

	private:
		std::string _basePath;
		std::vector<Asset*> _assets;
		AssetCatalog* _catalog;
	};
}
