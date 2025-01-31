#include "BrowserDialog.h"
#include "NewFolderDialog.h"
#include <ZEngine-Core/ImmediateUI/GUITextField.h>
#include <ZEngine-Core/ImmediateUI/GUINameGenerator.h>
#include <ZEngine-Core/Utilities/Directory.h>
#include <ZEngine-Core/Utilities/File.h>

#include <algorithm>

using namespace ZEngine;

BrowserDialog::BrowserDialog(std::string basePath, BrowserDialogType type) : GUIDialog(type == BROWSER_OPEN_FILE || type == BROWSER_OPEN_FOLDER ? "Open.." : "Save..", 800, 505, true)
{
	_rootDir = new Directory(basePath);
	_selectedDir = new Directory(basePath);
	_selectedFile = new File("");
	_type = type;
	_saveExtension = "";
	_userData = nullptr;

	_pathField = new GUITextField(GUINameGenerator::GetUniqueName("Path"));
	_nameField = new GUITextField(GUINameGenerator::GetUniqueName("Name"));
	_newFolderDiag = new NewFolderDialog(basePath);

	_pathField->SetText(_rootDir->GetAbsolutePath());

	Refresh();
}

void BrowserDialog::Refresh()
{
	_cachedDirs = _rootDir->GetAllDirectories();

	if (_filter.size() == 0)
		_cachedFiles = _rootDir->GetAllFiles();
	else
	{
		_cachedFiles.clear();

		for (const auto& extension : _filter)
		{
			auto files = _rootDir->GetAllFilesWithExtention(extension);
			_cachedFiles.insert(_cachedFiles.end(), files.begin(), files.end());
		}
	}
}

bool BrowserDialog::ValidateOpenFile()
{
	if (_selectedFile->GetFilename() != _nameField->GetText())
		_selectedFile->Set(_rootDir->GetPath() + _nameField->GetText());

	if (!_selectedFile->Exists())
		return false;

	if (!std::any_of(_filter.begin(), _filter.end(), [this](auto ext) { return _selectedFile->GetExtension() == ext; }))
		return false;

	return true;
}

bool BrowserDialog::ValidateSaveFile()
{
	if (_selectedFile->GetFilename() != _nameField->GetText())
		_selectedFile->Set(_rootDir->GetPath() + _nameField->GetText());

	if (_saveExtension != "")
	{
		if (_selectedFile->GetExtension() == "")
			_selectedFile->Set(_selectedFile->GetPath() + "." + _saveExtension);

		if (_selectedFile->GetExtension() != _saveExtension)
			return false;
	}

	return true;
}

bool BrowserDialog::ValidateSelectFolder()
{
	if (_selectedDir->GetName() != _nameField->GetText())
		_selectedDir->Set(_rootDir->GetPath() + _nameField->GetText());

	if (!_selectedDir->Exists())
		return false;

	return true;
}

void BrowserDialog::ProcessInput()
{
	if (_newFolderDiag->IsVisible())
	{
		if (_newFolderDiag->GetResult() == DIALOG_RESULT_CLOSE)
			_newFolderDiag->Hide();
		else if (_newFolderDiag->GetResult() == DIALOG_RESULT_OK)
		{
			_newFolderDiag->Hide();
			Refresh();
		}
	}
}

void BrowserDialog::RenderInWindow()
{
	_pathField->RenderElement();

	ImGui::Separator();

	if (ImGui::Button("New Folder"))
		_newFolderDiag->Show();

	ImGui::SameLine();
	if (ImGui::Button("Refresh"))
		Refresh();

	ImGui::Separator();

	ImGui::BeginChild("###browserchild", ImVec2(0, 270), true);

	ImGui::Columns(4);

	for (const auto& folder : _cachedDirs)
	{
		if (folder.GetName() == ".")
			continue;

		bool selected = _selectedDir->GetName() == folder.GetName();

		if (ImGui::Selectable((folder.GetName() + "/").c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick))
		{
			// Don't allow selecting a folder that doesn't exist.
			if (folder.GetName() != "..")
			{
				_selectedDir->Set(folder.GetPath());

				if (_type == BROWSER_OPEN_FOLDER)
					_nameField->SetText(folder.GetName());
			}

			if (ImGui::IsMouseDoubleClicked(0))
			{
				_rootDir->Set(folder.GetAbsolutePath());
				_selectedDir->Set(_rootDir->GetAbsolutePath());
				_selectedFile->Set("");
				_pathField->SetText(folder.GetAbsolutePath());

				if (_type == BROWSER_OPEN_FOLDER)
					_nameField->SetText(Directory(folder.GetAbsolutePath()).GetName());

				Refresh();
				break;
			}
		}

		ImGui::NextColumn();
	}

	// Only render files if selecting or saving files.
	if (_type != BROWSER_OPEN_FOLDER) {
		for (const auto& file : _cachedFiles)
		{
			bool selected = _selectedFile->GetFilename() == file.GetFilename();

			if (ImGui::Selectable(file.GetFilename().c_str(), selected, ImGuiSelectableFlags_AllowDoubleClick))
			{
				_selectedFile->Set(file.GetPath());
				_nameField->SetText(file.GetFilename());

				if (ImGui::IsMouseDoubleClicked(0))
				{
					if (_type == BROWSER_OPEN_FILE)
					{
						if (ValidateOpenFile())
							SetResult(DIALOG_RESULT_OK);
					}
				}
			}

			ImGui::NextColumn();
		}
	}

	ImGui::EndChild();

	_nameField->RenderElement();

	if (_type == BROWSER_SAVE_FILE)
	{
		if (ImGui::Button("Save"))
		{
			if (ValidateSaveFile())
				SetResult(DIALOG_RESULT_OK);
		}
	}
	else if (_type == BROWSER_OPEN_FILE)
	{
		if (ImGui::Button("Open"))
		{
			if (ValidateOpenFile())
				SetResult(DIALOG_RESULT_OK);
		}
	}
	else if (_type == BROWSER_OPEN_FOLDER)
	{
		if (ImGui::Button("Select folder"))
		{
			if (ValidateSelectFolder())
				SetResult(DIALOG_RESULT_OK);
		}
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Cancel"))
	{
		SetResult(DIALOG_RESULT_CLOSE);
	}

	_newFolderDiag->RenderElement();
}

void BrowserDialog::SetSaveExtension(std::string extension)
{
	_saveExtension = extension;
}

const std::string& BrowserDialog::GetSaveExtension() const
{
	return _saveExtension;
}

void BrowserDialog::SetUserData(void* data)
{
	_userData = data;
}

void* BrowserDialog::GetUserData() const
{
	return _userData;
}

void BrowserDialog::SetFilter(std::vector<std::string> extensions)
{
	_filter = extensions;
	Refresh();
}

const std::vector<std::string>& BrowserDialog::GetFilter() const
{
	return _filter;
}

void BrowserDialog::SetDirectory(std::string basePath)
{
	auto newDir = new Directory(basePath);

	if (newDir->Exists())
	{
		delete _rootDir;
		_rootDir = newDir;
	}
	else
		delete newDir;
}

const Directory& BrowserDialog::GetDirectory() const
{
	return *_rootDir;
}

const Directory& BrowserDialog::GetSelectedDirectory() const
{
	return *_selectedDir;
}

const File& BrowserDialog::GetFile() const
{
	return *_selectedFile;
}

const BrowserDialogType& BrowserDialog::GetBrowserType() const
{
	return _type;
}

BrowserDialog::~BrowserDialog()
{
	delete _rootDir;
	delete _selectedFile;
	delete _selectedDir;
	delete _pathField;
	delete _nameField;
}