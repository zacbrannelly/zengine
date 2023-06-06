#include "CreateScriptDialog.h"
#include "BrowserDialog.h"
#include "../UI/GUITextField.h"

#include "../Utilities/Directory.h"
#include "../Utilities/File.h"

#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>

#include <algorithm>
#include <sstream>

CreateScriptDialog::CreateScriptDialog(std::string basePath) : GUIDialog("Create Script", 500, 235, true)
{
	_basePath = basePath;
	_nameField = new GUITextField("Script Name");
	_pathField = new GUITextField("Script Path");

	_browser = new BrowserDialog("./", BROWSER_SAVE_FILE);
	_browser->SetFilter({ "js" });
	_browser->SetSaveExtension("js");
	Add(_browser);

	_pathField->SetText(basePath);

	_includeInit = true;
	_includeUpdate = true;
	_includeRender = true;

	SetFlags(ImGuiWindowFlags_NoResize);
}

bool CreateScriptDialog::Validate()
{
	auto name = _nameField->GetText();

	if (name == "")
		return false;
	
	// Check if name is all whitespace
	if (std::all_of(name.begin(), name.end(), [](const auto& c) { return c == ' '; }))
		return false;

	// Check if name contains non-alphanumeric characters (inc underscore)
	if (std::any_of(name.begin(), name.end(), [](const auto& c) { return !((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (c >= '0' && c <= '9') || c == '_'); }))
		return false;

	auto path = _pathField->GetText();

	if (path == "")
		return false;

	// Check if path is all whitespace
	if (std::all_of(path.begin(), path.end(), [](const auto& c) { return c == ' '; }))
		return false;

	return true;
}

std::string CreateScriptDialog::GenerateCode(const std::string& name)
{
	std::stringstream ss;

	ss << "function " << name << "()" << std::endl;
	ss << "{" << std::endl;
	ss << "\t" << "// TODO: Initialize variables here" << std::endl;
	ss << "}" << std::endl;
	ss << std::endl;

	if (_includeInit)
	{
		ss << name << ".prototype.Init = function()" << std::endl;
		ss << "{" << std::endl;
		ss << "\t" << "// TODO: Called once when the game starts" << std::endl;
		ss << "}" << std::endl;
		ss << std::endl;
	}

	if (_includeUpdate)
	{
		ss << name << ".prototype.Update = function()" << std::endl;
		ss << "{" << std::endl;
		ss << "\t" << "// TODO: Called every frame" << std::endl;
		ss << "}" << std::endl;
		ss << std::endl;
	}

	if (_includeRender)
	{
		ss << name << ".prototype.Render = function()" << std::endl;
		ss << "{" << std::endl;
		ss << "\t" << "// TODO: Called every frame, do drawing here" << std::endl;
		ss << "}" << std::endl;
	}

	return ss.str();
}

bool CreateScriptDialog::Create(bool import)
{
	if (Validate())
	{
		auto name = _nameField->GetText();
		auto path = _pathField->GetText();

		File scriptFile(path);

		if (!scriptFile.Exists())
		{
			if (!scriptFile.Create())
				return false;
		}

		auto code = GenerateCode(name);

		if (!scriptFile.WriteContent(&code[0], code.size()))
			return false;

		if (import)
		{
			auto catalog = AssetManager::GetInstance()->GetCatalog();

			if (catalog != nullptr)
			{
				catalog->RegisterAsset(path, CSHARP_SCRIPT_ASSET);
				catalog->SaveCatalog(catalog->GetLastCatalogPath());
			}
		}

		return true;
	}

	return false;
}

void CreateScriptDialog::ProcessInput()
{
	if (GetResult() == DIALOG_RESULT_CLOSE)
		Close();

	if (_browser->IsVisible())
	{
		if (_browser->GetResult() == DIALOG_RESULT_CLOSE)
			_browser->Hide();
		else if (_browser->GetResult() == DIALOG_RESULT_OK)
		{
			// Retrieve paths from the browser
			_pathField->SetText(_browser->GetFile().GetRelativePath());
			_nameField->SetText(_browser->GetFile().GetName());
			_basePath = _browser->GetDirectory().GetPathRelativeTo();

			_browser->Hide();
		}
	}

	if (_nameField->GetText() != "")
		_pathField->SetText(_basePath + _nameField->GetText() + ".js");
	else
		_pathField->SetText(_basePath);

	if (_pathField->GetText() != _basePath + _nameField->GetText() + ".js")
	{
		_basePath = Directory::GetBasePath(_pathField->GetText());
	}
}

void CreateScriptDialog::RenderInWindow()
{
	_nameField->RenderElement();

	if (ImGui::Button("..."))
	{
		_browser->SetDirectory(_pathField->GetText());
		_browser->Show();
	}

	ImGui::SameLine();
	_pathField->RenderElement();

	ImGui::Separator();

	ImGui::Text("Include the following functions:");
	ImGui::Checkbox("Init function", &_includeInit);
	ImGui::Checkbox("Update function", &_includeInit);
	ImGui::Checkbox("Render function", &_includeInit);

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

CreateScriptDialog::~CreateScriptDialog()
{
	delete _nameField;
	delete _pathField;
}