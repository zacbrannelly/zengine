#include "NewFolderDialog.h"
#include <ZEngine-Core/Utilities/Directory.h>
#include "../UI/GUITextField.h"

#include <algorithm>

using namespace std;

NewFolderDialog::NewFolderDialog(string basePath) : GUIDialog("New Folder..", 300, 100, true)
{
	_basePath = basePath;
	_nameField = new GUITextField("Folder Name");
	_nameField->SetText("New_Folder");
}

bool NewFolderDialog::Validate()
{
	auto name = _nameField->GetText();

	// Empty
	if (name == "")
		return false;

	// All whitespace
	if (all_of(name.begin(), name.end(), [](const auto& c) { return c == ' '; }))
		return false;

	// Illegal characters
	if (any_of(name.begin(), name.end(), [](const auto& c) { return c == '/' || c == '\\'; }))
		return false;

	return true;
}

void NewFolderDialog::ProcessInput()
{

}

void NewFolderDialog::RenderInWindow()
{
	_nameField->RenderElement();

	if (ImGui::Button("Create"))
	{
		if (Validate())
		{
			Directory dir(_basePath + _nameField->GetText());

			if (!dir.Exists())
			{
				if (dir.Create())
					SetResult(DIALOG_RESULT_OK);
			}
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Close"))
	{
		SetResult(DIALOG_RESULT_CLOSE);
	}
}

void NewFolderDialog::SetBase(string path)
{
	_basePath = path;
}

const string& NewFolderDialog::GetBase() const
{
	return _basePath;
}

string NewFolderDialog::GetResultPath() const 
{
	return _basePath + _nameField->GetText();
}

NewFolderDialog::~NewFolderDialog()
{
	delete _nameField;
}