#include "BrowserDialog.h"

namespace ZEngine
{
  class Editor;

  class ProjectBrowserDialog : public BrowserDialog
  {
  public:
    ProjectBrowserDialog(Editor* editor);
    ~ProjectBrowserDialog();

    void RenderInWindow() override;

  private:
    Editor* _editor;
  };
}
