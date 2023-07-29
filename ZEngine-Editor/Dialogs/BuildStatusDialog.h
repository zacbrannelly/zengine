#pragma once

#include "../UI/GUIDialog.h"

class Editor;

class BuildStatusDialog : public GUIDialog
{
public:
	BuildStatusDialog(Editor* editor);
	~BuildStatusDialog();

  void ProcessInput() override;
	void RenderInWindow() override;
  void RenderElement() override;

private:
  Editor* _editor;
};
