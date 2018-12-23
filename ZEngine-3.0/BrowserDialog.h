#pragma once

#include "GUIDialog.h"

class File;
class Directory;
class GUITextField;
class NewFolderDialog;

enum BrowserDialogType
{
	BROWSER_OPEN_FILE,
	BROWSER_OPEN_FOLDER,
	BROWSER_SAVE_FILE,
};

class BrowserDialog : public GUIDialog
{
public:
	BrowserDialog(std::string basePath = "./", BrowserDialogType type = BROWSER_OPEN_FILE);
	~BrowserDialog();

	void SetFilter(std::vector<std::string> extensions);
	const std::vector<std::string>& GetFilter() const;
	
	void SetSaveExtension(std::string extension);
	const std::string& GetSaveExtension() const;

	void Refresh();

	void ProcessInput() override;
	void RenderInWindow() override;

	void SetDirectory(std::string basePath);

	const File& GetFile() const;
	const Directory& GetDirectory() const;
	const Directory& GetSelectedDirectory() const;
	
	const BrowserDialogType& GetBrowserType() const;

private:
	bool ValidateOpenFile();
	bool ValidateSaveFile();
	bool ValidateSelectFolder();

	Directory* _rootDir;
	File* _selectedFile;
	Directory* _selectedDir;
	BrowserDialogType _type;
	std::string _saveExtension;

	GUITextField* _pathField;
	GUITextField* _nameField;
	NewFolderDialog* _newFolderDiag;

	std::vector<Directory> _cachedDirs;
	std::vector<File> _cachedFiles;
	std::vector<std::string> _filter;
};