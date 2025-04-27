#include "CreateMaterialDialog.h"
#include "BrowserDialog.h"
#include "../Editor.h"
#include "../Project/Project.h"
#include <ZEngine-Core/ImmediateUI/GUITextField.h>
#include <ZEngine-Core/Utilities/Directory.h>
#include <ZEngine-Core/Utilities/File.h>

#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Rendering/StandardShaders.h>
#include <ZEngine-Core/Rendering/Shader.h>
#include <nlohmann/json.hpp>
#include <algorithm>

using namespace ZEngine;

CreateMaterialDialog::CreateMaterialDialog(Editor* editor, std::string basePath) : GUIDialog("Create Material", 600, 300, true)
{
	_editor = editor;
	_basePath = basePath;

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

	if (_shaderID.is_nil() && _standardShader == nullptr)
		return false;

	return true;
}

std::string CreateMaterialDialog::GenerateJSON()
{
	using namespace nlohmann;

	json root;

	root["name"] = _nameField->GetText();
	root["shader"] = json::object_t();

	if (_standardShader != nullptr)
	{
		root["shader"]["standardShader"] = _standardShader->enumName;
	}
	else
	{
		root["shader"]["assetId"] = uuids::to_string(_shaderID);
	}

	return root.dump(4);
}

bool CreateMaterialDialog::Create()
{
	if (Validate())
	{
		File assetFile(_pathField->GetText());
		auto jsonCode = GenerateJSON();

		if (!assetFile.WriteContent(&jsonCode[0], jsonCode.size()))
			return false;

		_editor->GetProject()->GetCatalog().RegisterAsset(_pathField->GetText(), MATERIAL_ASSET);
		_editor->GetProject()->Save();
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
		_pathField->SetText(_basePath + _nameField->GetText() + ".material");
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

	if (!_shaderID.is_nil() && catalog != nullptr)
	{
		ObjectType type;
		catalog->GetAssetPathFromID(_shaderID, shaderName, type);
	}

	if (_standardShader != nullptr)
	{
		shaderName = _standardShader->shader->GetName();
	}

	if (ImGui::BeginCombo("Shader", shaderName.c_str()))
	{
		if (catalog != nullptr)
		{
			// Standard shaders
			for (auto& standardShader : StandardShaders::GetShaders())
			{
				auto shader = standardShader.shader;
				if (ImGui::Selectable(shader->GetName().c_str()))
				{
					_standardShader = &standardShader;
					_shaderID = uuids::uuid();
				}
			}

			// User shaders
			auto availShaders = catalog->GetAssetsByType(SHADER_ASSET);
			for (const auto& shader : availShaders)
			{
				if (ImGui::Selectable(shader.name.c_str()))
				{
					_standardShader = nullptr;
					_shaderID = shader.id;
				}
			}
		}

		ImGui::EndCombo();
	}

	ImGui::Separator();

	if (ImGui::Button("Create"))
	{
		if (Create())
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