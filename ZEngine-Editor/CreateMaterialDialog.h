#pragma once

#include "GUIDialog.h"

class GUITextField;
class BrowserDialog;

class CreateMaterialDialog : public GUIDialog
{
public:
	CreateMaterialDialog(std::string basePath = "./");
	~CreateMaterialDialog();

	void ProcessInput() override;
	void RenderInWindow() override;
	
private:
	std::string GenerateJSON();
	bool Validate();
	bool Create(bool import);

	GUITextField* _nameField;
	GUITextField* _pathField;
	BrowserDialog* _browser;
	std::string _basePath;
	int _shaderID;
};
