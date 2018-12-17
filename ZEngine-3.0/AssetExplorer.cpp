#include "AssetExplorer.h"
#include "File.h"
#include "Editor.h"
#include "CodeEditor.h"
#include "imgui-includes.h"

#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>

AssetExplorer::AssetExplorer(Editor* editor) : GUIWindow("Asset Explorer", 300, 150, false)
{
	_editor = editor;
	_manager = AssetManager::GetInstance();
}

void AssetExplorer::ProcessInput()
{

}

void AssetExplorer::RenderAssetButtons(const std::vector<CatalogEntry>& items)
{
	bool selected = false;
	for (const auto& item : items)
	{
		File file(item.path);

		selected = _selectedName == file.GetFilename();

		if (ImGui::Selectable(file.GetFilename().c_str(), &selected, ImGuiSelectableFlags_AllowDoubleClick))
		{
			_selectedName = file.GetFilename();

			if (ImGui::IsMouseDoubleClicked(0))
			{
				// TODO: Launch asset specific actions

				if (item.type == SCRIPT_ASSET)
				{
					bool alreadyOpen = false;

					for (auto element : _editor->GetElements())
					{
						if (element->GetType() == GUI_TYPE_CODE_EDITOR)
						{
							auto editor = static_cast<CodeEditor*>(element);

							if (editor->GetFile().GetPath() == item.path)
							{
								alreadyOpen = true;
								editor->Focus();
								break;
							}
						}
					}

					if (!alreadyOpen)
						_editor->Add(new CodeEditor(item.path));
				}
			}
		}
	}
}

void AssetExplorer::RenderInWindow()
{
	if (_manager->GetCatalog() == nullptr)
		return;

	auto audioAssets = _manager->GetCatalog()->GetAssetsByType(AUDIO_ASSET);
	auto mapAssets = _manager->GetCatalog()->GetAssetsByType(MAP_ASSET);
	auto materialAssets = _manager->GetCatalog()->GetAssetsByType(MATERIAL_ASSET);
	auto modelAssets = _manager->GetCatalog()->GetAssetsByType(MODEL_ASSET);
	auto scriptAssets = _manager->GetCatalog()->GetAssetsByType(SCRIPT_ASSET);
	auto shaderAssets = _manager->GetCatalog()->GetAssetsByType(SHADER_ASSET);
	auto textureAssets = _manager->GetCatalog()->GetAssetsByType(TEXTURE_ASSET);

	if (ImGui::TreeNode("Sounds"))
	{
		RenderAssetButtons(audioAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Maps"))
	{
		RenderAssetButtons(mapAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Materials"))
	{
		RenderAssetButtons(materialAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Models"))
	{
		RenderAssetButtons(modelAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Scripts"))
	{
		RenderAssetButtons(scriptAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Shaders"))
	{
		RenderAssetButtons(shaderAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Textures"))
	{
		RenderAssetButtons(textureAssets);
		ImGui::TreePop();
	}
}

AssetExplorer::~AssetExplorer()
{
}
