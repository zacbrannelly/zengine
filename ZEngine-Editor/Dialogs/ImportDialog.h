#pragma once
#include "BrowserDialog.h"
#include <ZEngine-Core/Utilities/Directory.h>

namespace ZEngine
{
  class Editor;

  class ImportDialog : public BrowserDialog
  {
  public:
    ImportDialog(Editor* editor, Directory targetDirectory);
    ~ImportDialog();

    void RenderInWindow() override;

  private:
    Editor* _editor;
    Directory _targetDirectory;
  };
}
