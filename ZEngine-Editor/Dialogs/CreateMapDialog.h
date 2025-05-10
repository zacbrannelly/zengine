#pragma once

#include <ZEngine-Core/ImmediateUI/GUIDialog.h>

namespace ZEngine
{
	class GUITextField;
	class Editor;

	class CreateMapDialog : public GUIDialog
	{
	public:
		CreateMapDialog(Editor* editor, std::string basePath = "./");
		~CreateMapDialog();

		void ProcessInput() override;
		void RenderInWindow() override;

	private:
		bool Validate();
		bool Create();

		Editor* _editor;
		GUITextField* _nameField;
		std::string _basePath;
	};
}
