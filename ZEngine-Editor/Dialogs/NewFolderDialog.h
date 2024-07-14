#pragma once

#include <ZEngine-Core/ImmediateUI/GUIDialog.h>

class GUITextField;

class NewFolderDialog : public GUIDialog
{
public:
	NewFolderDialog(std::string basePath);
	~NewFolderDialog();

	std::string GetResultPath() const;

	void ProcessInput() override;
	void RenderInWindow() override;

	void SetBase(std::string path);
	const std::string& GetBase() const;

private:
	bool Validate();

	std::string _basePath;
	GUITextField* _nameField;
};