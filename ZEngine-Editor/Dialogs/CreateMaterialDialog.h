#pragma once

#include <ZEngine-Core/ImmediateUI/GUIDialog.h>
#include <uuid.h>

namespace ZEngine
{
	class GUITextField;
	class BrowserDialog;
	class Editor;
	class ShaderInfo;

	class CreateMaterialDialog : public GUIDialog
	{
	public:
		CreateMaterialDialog(Editor* editor, std::string basePath = "./");
		~CreateMaterialDialog();

		void ProcessInput() override;
		void RenderInWindow() override;
		
	private:
		std::string GenerateJSON();
		bool Validate();
		bool Create();

		Editor* _editor;
		GUITextField* _nameField;
		GUITextField* _pathField;
		BrowserDialog* _browser;
		std::string _basePath;
		uuids::uuid _shaderID;
		ShaderInfo const* _standardShader { nullptr };
	};
}
