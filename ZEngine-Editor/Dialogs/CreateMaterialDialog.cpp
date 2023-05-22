#include "CreateMaterialDialog.h"
#include "BrowserDialog.h"
#include "../UI/GUITextField.h"
#include "../Utilities/Directory.h"
#include "../Utilities/File.h"

#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <nlohmann/json.hpp>
#include <algorithm>

CreateMaterialDialog::CreateMaterialDialog(std::string basePath) : GUIDialog("Create Material", 500, 155, true)
{
	_basePath = basePath;
	_shaderID = -1;

	_nameField = new GUITextField("Name");
	_pathField = new GUITextField("Path");
	_browser = new BrowserDialog("./", BROWSER_SAVE_FILE);
	_browser->SetFilter({ "asset" });
	_browser->SetSaveExtension("asset");
}

bool CreateMaterialDialog::Validate()
{
	auto name = _nameField->GetText();

	if (name == "")
		return false;

	if (std::all_of(name.begin(), name.end(), [](const auto& c) { return c == ' '; }))
		return false;

	if (_shaderID < 0)
		return false;

	return true;
}

std::string CreateMaterialDialog::GenerateJSON()
{
	using namespace nlohmann;

	json root;

	root["name"] = _nameField->GetText();
	root["shader"] = _shaderID;

	return root.dump(4);
}

bool CreateMaterialDialog::Create(bool import)
{
	if (Validate())
	{
		File assetFile(_pathField->GetText());
		auto jsonCode = GenerateJSON();

		if (!assetFile.WriteContent(&jsonCode[0], jsonCode.size()))
			return false;

		if (import)
		{
			auto catalog = AssetManager::GetInstance()->GetCatalog();

			if (catalog != nullptr)
			{
				catalog->RegisterAsset(_pathField->GetText(), MATERIAL_ASSET);
				catalog->SaveCatalog(catalog->GetLastCatalogPath());
			}
		}

		return true;
	}

	return false;
}

void CreateMaterialDialog::ProcessInput()
{
	if (GetResult() == DIALOG_RESULT_CLOSE)
		Close();

	if (_browser->IsVisible())
	{
		if (_browser->GetResult() == DIALOG_RESULT_OK)
		{
			_basePath = _browser->GetDirectory().GetPathRelativeTo();
			_nameField->SetText(_browser->GetFile().GetName());
			_pathField->SetText(_browser->GetFile().GetRelativePath());

			_browser->Hide();
		}
		else if (_browser->GetResult() == DIALOG_RESULT_CLOSE)
			_browser->Hide();
	}

	if (_nameField->GetText() != "")
		_pathField->SetText(_basePath + _nameField->GetText() + ".asset");
	else
		_pathField->SetText(_basePath);
}

void CreateMaterialDialog::RenderInWindow()
{
	_browser->RenderElement();

	_nameField->RenderElement();

	if (ImGui::Button("..."))
	{
		_browser->Show();
	}

	ImGui::SameLine();
	_pathField->RenderElement();

	auto catalog = AssetManager::GetInstance()->GetCatalog();
	std::string shaderName = "None";

	if (_shaderID >= 0 && catalog != nullptr)
	{
		ObjectType type;
		catalog->GetAssetPathFromID(_shaderID, shaderName, type);
	}

	if (ImGui::BeginCombo("Shader", shaderName.c_str()))
	{
		if (catalog != nullptr)
		{
			auto availShaders = catalog->GetAssetsByType(SHADER_ASSET);

			for (const auto& shader : availShaders)
			{
				if (ImGui::Selectable(shader.path.c_str()))
				{
					_shaderID = shader.id;
				}
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();

	if (ImGui::Button("Create"))
	{
		if (Create(false))
			Close();
	}

	ImGui::SameLine();
	if (ImGui::Button("Create & Import"))
	{
		if (Create(true))
			Close();
	}

	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
		Close();
}

CreateMaterialDialog::~CreateMaterialDialog()
{
	delete _nameField;
	delete _pathField;
	delete _browser;
}