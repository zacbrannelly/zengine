#pragma once

#include <vector>
#include <map>
#include <string> 
#include <ZEngine-Core/Map/Objects/ZObject.h>
#include <ZEngine-Core/ImmediateUI/GUIWindow.h>

namespace ZEngine
{
	class File;
	class Directory;
	class AssetManager;

	class AssetImporter : public GUIWindow
	{
	public:
		AssetImporter();
		~AssetImporter();

		void Refresh();

		void ProcessInput() override;
		void RenderInWindow() override;

	private:
		AssetManager* _assetManager;
		Directory* _rootFolder;
		std::map<std::string, ObjectType> _validExtensions;
		std::vector<Directory> _cachedFolders;
		std::vector<File> _cachedFiles;
		std::string _selectedName;
	};
}
