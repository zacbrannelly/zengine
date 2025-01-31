#pragma once

#include <ZEngine-Core/ImmediateUI/GUIDialog.h>

namespace ZEngine
{
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
}
