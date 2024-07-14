#pragma once

#include <ZEngine-Core/ImmediateUI/GUIDialog.h>

class GUITextField;
class BrowserDialog;

class CreateShaderDialog : public GUIDialog
{
public:
	CreateShaderDialog(std::string basePath = "./");
	~CreateShaderDialog();

	void ProcessInput() override;
	void RenderInWindow() override;

private:
	std::string GenerateVertexCode() const;
	std::string GenerateFragmentCode() const;
	std::string GenerateVaryingDefCode() const;
	std::string GenerateJSON() const;

	bool Validate();
	bool ValidatePath(const std::string& path, std::string prefExtension);
	bool Create(bool import);

	GUITextField* _nameField;
	GUITextField* _assetPathField;
	BrowserDialog* _browser;
	std::string _basePath;
	int _numberOfPasses;
};
