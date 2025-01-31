#include "ProjectBrowserDialog.h"
#include "../Project/Project.h"
#include <ZEngine-Core/Utilities/File.h>
#include "../Editor.h"

using namespace ZEngine;

ProjectBrowserDialog::ProjectBrowserDialog(Editor* editor) : BrowserDialog(), _editor(editor)
{
  SetTitle("Open Project");
  SetModal(true);
  SetFilter({ "zproj" });
  SetVisible(true);
}

void ProjectBrowserDialog::RenderInWindow()
{
  BrowserDialog::RenderInWindow();

  if (IsVisible())
  {
    auto result = GetResult();
    if (result == DIALOG_RESULT_CLOSE)
    {
      Close();
    }
    else if (result == DIALOG_RESULT_OK)
    {
      const auto file = GetFile();
      const auto path = file.GetPath();

      const auto project = new Project();
      project->Load(path);
      _editor->SetProject(project);

      Close();
    }
  }
}

ProjectBrowserDialog::~ProjectBrowserDialog()
{
}
