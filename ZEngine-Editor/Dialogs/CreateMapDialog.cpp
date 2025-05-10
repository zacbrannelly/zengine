#include "CreateMapDialog.h"
#include <ZEngine-Core/ImmediateUI/GUITextField.h>
#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Utilities/Directory.h>
#include "../Builders/MapBuilder.h"
#include "../Project/Project.h"
#include "../Editor.h"

#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <algorithm>

using namespace ZEngine;

CreateMapDialog::CreateMapDialog(Editor* editor, std::string basePath) : GUIDialog("Create Map", 500, 200, true)
{
	_editor = editor;
	_basePath = basePath;
	_nameField = new GUITextField("Name");
}

bool CreateMapDialog::Validate()
{
	auto name = _nameField->GetText();

	if (name == "")
		return false;

	if (std::any_of(name.begin(), name.end(), [](const auto& c) { return c == ' '; }))
		return false;
	
	return true;
}

bool CreateMapDialog::Create()
{
	if (Validate())
	{
		auto name = _nameField->GetText();
		auto path = _basePath + name + ".map";

		if (!MapBuilder::BuildDefault(path, name))
		{
			return false;
		}

		auto project = _editor->GetProject();
		if (project != nullptr)
		{
			auto& catalog = project->GetCatalog();
			catalog.RegisterAsset(path, MAP_ASSET);
			project->Save();
		}

		return true;
	}

	return false;
}

void CreateMapDialog::ProcessInput()
{
	if (GetResult() == DIALOG_RESULT_CLOSE)
		Close();
}

void CreateMapDialog::RenderInWindow()
{
	_nameField->RenderElement();

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

CreateMapDialog::~CreateMapDialog()
{
	delete _nameField;
}
