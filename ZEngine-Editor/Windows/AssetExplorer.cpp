#include "AssetExplorer.h"
#include <ZEngine-Core/Utilities/File.h>
#include "../Editor.h"
#include "CodeEditor.h"
#include "ShaderEditor.h"
#include "MaterialEditor.h"
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>

#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/Objects/MapAsset.h>
#include <ZEngine-Core/Assets/Objects/CSharpScriptAsset.h>

using namespace ZEngine;

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
				OpenAsset(item.path, (ObjectType)item.type);
			}
		}
	}
}

void AssetExplorer::OpenAsset(std::string path, ObjectType type)
{
	if (type == CSHARP_SCRIPT_ASSET)
	{
		for (auto element : _editor->GetElements())
		{
			if (element->GetType() == GUI_TYPE_CODE_EDITOR)
			{
				auto editor = static_cast<CodeEditor*>(element);

				if (editor->GetFile().GetPath() == path)
				{
					editor->Focus();
					return;
				}
			}
		}

		auto scriptAsset = _manager->FindAssetFromPath(path);
		if (scriptAsset != nullptr) {
			auto scriptPath = scriptAsset->Cast<CSharpScriptAsset>()->GetScriptPath();
			_editor->Add(new CodeEditor(scriptPath));
		}
	}
	else if (type == MAP_ASSET)
	{
		auto mapAsset = _manager->FindAssetFromPath(path);

		if (mapAsset == nullptr)
			mapAsset = _manager->LoadAsset("", path, MAP_ASSET);

		if (mapAsset != nullptr)
		{
			auto map = mapAsset->Cast<MapAsset>()->GetMap();
			
			if (map != _editor->GetSelectedMap())
			{
				_editor->SetSelectedMap(map);
			}
		}
	}
	else if (type == SHADER_ASSET)
	{
		for (auto& element : _editor->GetElements())
		{
			if (element->GetType() == GUI_TYPE_SHADER_EDITOR)
			{
				auto shaderEditor = static_cast<ShaderEditor*>(element);
				
				if (shaderEditor->GetAssetPath() == path)
				{
					shaderEditor->Focus();
					return;
				}
			}
		}

		_editor->Add(new ShaderEditor(path));
	}
	else if (type == MATERIAL_ASSET)
	{
		for (auto& element : _editor->GetElements())
		{
			if (element->GetType() == GUI_TYPE_MATERIAL_EDITOR)
			{
				auto matEditor = static_cast<MaterialEditor*>(element);

				if (matEditor->GetAssetPath() == path)
				{
					matEditor->Focus();
					return;
				}
			}
		}

		_editor->Add(new MaterialEditor(path));
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
	auto scriptAssets = _manager->GetCatalog()->GetAssetsByType(CSHARP_SCRIPT_ASSET);
	auto shaderAssets = _manager->GetCatalog()->GetAssetsByType(SHADER_ASSET);
	auto textureAssets = _manager->GetCatalog()->GetAssetsByType(TEXTURE_ASSET);

	auto flags = ImGuiTreeNodeFlags_FramePadding;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 5));

	if (ImGui::TreeNodeEx("Sounds", flags))
	{
		RenderAssetButtons(audioAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Maps", flags))
	{
		RenderAssetButtons(mapAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Materials", flags))
	{
		RenderAssetButtons(materialAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Models", flags))
	{
		RenderAssetButtons(modelAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Scripts", flags))
	{
		RenderAssetButtons(scriptAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Shaders", flags))
	{
		RenderAssetButtons(shaderAssets);
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Textures", flags))
	{
		RenderAssetButtons(textureAssets);
		ImGui::TreePop();
	}

	ImGui::PopStyleVar();
}

AssetExplorer::~AssetExplorer()
{
}
