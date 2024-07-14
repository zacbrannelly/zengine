#pragma once

#include <ZEngine-Core/ImmediateUI/GUIDialog.h>

class BrowserDialog;
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
	BrowserDialog* _browser;

	bool _includeInit;
	bool _includeUpdate;
	bool _includeRender;
};