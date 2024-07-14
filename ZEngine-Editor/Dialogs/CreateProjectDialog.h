#pragma once

#include <ZEngine-Core/ImmediateUI/GUIDialog.h>

class GUITextField;
class BrowserDialog;
class Editor;

class CreateProjectDialog : public GUIDialog
{
public:
  CreateProjectDialog(Editor* editor, std::string basePath = "./");
  ~CreateProjectDialog();

  void ProcessInput() override;
  void RenderInWindow() override;
  void RenderElement() override;

private:
  bool Validate();
  bool Create();

  Editor* _editor;
  GUITextField* _nameField;
  GUITextField* _pathField;
  BrowserDialog* _browser;
  std::string _basePath;
};
