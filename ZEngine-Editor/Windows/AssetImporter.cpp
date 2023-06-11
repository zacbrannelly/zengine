#include "AssetImporter.h"
#include "ShaderImporter.h"
#include <ZEngine-Core/Utilities/Directory.h>
#include <ZEngine-Core/Utilities/File.h>
#include "../imgui-includes.h"

#include "../Dialogs/CreateScriptDialog.h"
#include "../Dialogs/CreateShaderDialog.h"
#include "../Dialogs/CreateMaterialDialog.h"
#include "../Dialogs/CreateMapDialog.h"

#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>

#define REGISTER_EXTENSION(x, y) _validExtensions[x] = y

AssetImporter::AssetImporter() : GUIWindow("Asset Importer", 200, 150, false)
{
	REGISTER_EXTENSION("png", TEXTURE_ASSET);
	REGISTER_EXTENSION("jpg", TEXTURE_ASSET);
	REGISTER_EXTENSION("map", MAP_ASSET);
	REGISTER_EXTENSION("shader", SHADER_ASSET);
	REGISTER_EXTENSION("cs", CSHARP_SCRIPT_ASSET);
	REGISTER_EXTENSION("wav", AUDIO_ASSET);
	REGISTER_EXTENSION("mp3", AUDIO_ASSET);
	REGISTER_EXTENSION("obj", MODEL_ASSET);

	_assetManager = AssetManager::GetInstance();
	_rootFolder = new Directory("./");

	Refresh();
	SetFlags(ImGuiWindowFlags_MenuBar);
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
	ImGui::BeginMenuBar();
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Map"))
			{
				auto dialog = new CreateMapDialog(_rootFolder->GetPath());
				dialog->Show();

				Add(dialog);
			}

			if (ImGui::MenuItem("Script"))
			{
				auto dialog = new CreateScriptDialog(_rootFolder->GetPath());
				dialog->Show();

				Add(dialog);
			}

			if (ImGui::MenuItem("Shader"))
			{
				auto dialog = new CreateShaderDialog(_rootFolder->GetPath());
				dialog->Show();

				Add(dialog);
			}

			if (ImGui::MenuItem("Material"))
			{
				auto dialog = new CreateMaterialDialog(_rootFolder->GetPath());
				dialog->Show();

				Add(dialog);
			}

			ImGui::EndMenu();
		}

	}
	ImGui::EndMenuBar();

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

				auto extension = Directory::GetExtension(file.GetPath());

				if (extension == "sc")
				{
					Add(new ShaderImporter(this, file.GetPath()));
				}
			}
		}

		ImGui::NextColumn();
	}

	if (newRoot != nullptr)
	{
		if (_rootFolder != nullptr)
			delete _rootFolder;

		_rootFolder = newRoot;
		Refresh();
	}
}

AssetImporter::~AssetImporter()
{
	if (_rootFolder != nullptr)
		delete _rootFolder;
}
