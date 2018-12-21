#pragma once

#include "GUIDialog.h"

class GUITextField;

class CreateScriptDialog : public GUIDialog
{
public:
	CreateScriptDialog(std::string basePath = "");
	~CreateScriptDialog();

	void ProcessInput() override;
	void RenderInWindow() override;

private:
	std::string GenerateCode(const std::string& name);
	bool Validate();
	bool Create(bool import);


	std::string _basePath;
	GUITextField* _nameField;
	GUITextField* _pathField;

	bool _includeInit;
	bool _includeUpdate;
	bool _includeRender;
};