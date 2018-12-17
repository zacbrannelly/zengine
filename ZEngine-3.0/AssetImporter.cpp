#include "AssetImporter.h"
#include "Directory.h"
#include "File.h"
#include "imgui-includes.h"

#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>

AssetImporter::AssetImporter() : GUIWindow("Asset Importer", 200, 150, false)
{
	_assetManager = AssetManager::GetInstance();
	_rootFolder = new Directory("./");

	Refresh();
}

void AssetImporter::Refresh()
{
	_cachedFiles = _rootFolder->GetAllFiles();
	_cachedFolders = _rootFolder->GetAllDirectories();
}

void AssetImporter::ProcessInput()
{

}

void AssetImporter::RenderInWindow()
{
	ImGui::Columns(2, "asset_importer", false);

	Directory* newRoot = nullptr;
	bool selected = false;

	for (const auto& folder : _cachedFolders)
	{
		selected = folder.GetName() == _selectedName;
		if (ImGui::Selectable(folder.GetName().c_str(), &selected, ImGuiSelectableFlags_AllowDoubleClick))
		{
			_selectedName = folder.GetName();

			if (ImGui::IsMouseDoubleClicked(0))
			{
				newRoot = new Directory(folder.GetPath());
			}
		}

		ImGui::NextColumn();
	}

	for (const auto& file : _cachedFiles)
	{
		selected = file.GetFilename() == _selectedName;
		if (ImGui::Selectable(file.GetFilename().c_str(), &selected, ImGuiSelectableFlags_AllowDoubleClick))
		{
			_selectedName = file.GetFilename();

			if (ImGui::IsMouseDoubleClicked(0))
			{
				// TODO: Attempt to import the file
			}
		}

		ImGui::NextColumn();
	}

	if (newRoot != nullptr)
	{
		_rootFolder = newRoot;
		Refresh();
	}
}

AssetImporter::~AssetImporter()
{
	if (_rootFolder != nullptr)
		delete _rootFolder;
}
