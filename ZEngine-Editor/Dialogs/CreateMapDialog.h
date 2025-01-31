#pragma once

#include <ZEngine-Core/ImmediateUI/GUIDialog.h>

namespace ZEngine
{
	class GUITextField;
	class BrowserDialog;

	class CreateMapDialog : public GUIDialog
	{
	public:
		CreateMapDialog(std::string basePath = "./");
		~CreateMapDialog();

		void ProcessInput() override;
		void RenderInWindow() override;

	private:
		bool Validate();
		bool Create(bool import);

		GUITextField* _nameField;
		GUITextField* _pathField;
		BrowserDialog* _browser;
		std::string _basePath;
	};
}
