#include "CreateMapDialog.h"
#include "BrowserDialog.h"
#include "../UI/GUITextField.h"
#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Utilities/Directory.h>
#include "../Builders/MapBuilder.h"

#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <algorithm>

CreateMapDialog::CreateMapDialog(std::string basePath) : GUIDialog("Create Map", 500, 200, true)
{
	_basePath = basePath;

	_nameField = new GUITextField("Name");
	_pathField = new GUITextField("Path");
	_browser = new BrowserDialog(basePath, BROWSER_SAVE_FILE);
	_browser->SetFilter({ "map" });
	_browser->SetSaveExtension("map");
}

bool CreateMapDialog::Validate()
{
	auto name = _nameField->GetText();

	if (name == "")
		return false;

	if (std::any_of(name.begin(), name.end(), [](const auto& c) { return c == ' '; }))
		return false;

	auto path = _pathField->GetText();

	if (path == "")
		return false;

	if (std::any_of(path.begin(), path.end(), [](const auto& c) { return c == ' '; }))
		return false;
	
	return true;
}

bool CreateMapDialog::Create(bool import)
{
	if (Validate())
	{
		auto name = _nameField->GetText();
		auto path = _pathField->GetText();

		if (!MapBuilder::BuildDefault(path, name))
		{
			return false;
		}

		if (import)
		{
			auto catalog = AssetManager::GetInstance()->GetCatalog();
			
			if (catalog != nullptr)
			{
				catalog->RegisterAsset(path, MAP_ASSET);
				catalog->SaveCatalog(catalog->GetLastCatalogPath());
			}
		}

		return true;
	}

	return false;
}

void CreateMapDialog::ProcessInput()
{
	if (GetResult() == DIALOG_RESULT_CLOSE)
		Close();

	if (_browser->IsVisible())
	{
		if (_browser->GetResult() == DIALOG_RESULT_OK)
		{
			_nameField->SetText(_browser->GetFile().GetName());
			_pathField->SetText(_browser->GetFile().GetRelativePath());
			_basePath = _browser->GetDirectory().GetPathRelativeTo();

			_browser->Hide();
		}
		else if (_browser->GetResult() == DIALOG_RESULT_CLOSE)
			_browser->Hide();
	}

	if (_nameField->GetText() != "")
		_pathField->SetText(_basePath + _nameField->GetText() + ".map");
	else
		_pathField->SetText(_basePath);
}

void CreateMapDialog::RenderInWindow()
{
	_browser->RenderElement();

	_nameField->RenderElement();

	if (ImGui::Button("..."))
	{
		_browser->Show();
	}

	ImGui::SameLine();
	_pathField->RenderElement();

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

CreateMapDialog::~CreateMapDialog()
{
	delete _nameField;
	delete _pathField;
	delete _browser;
}