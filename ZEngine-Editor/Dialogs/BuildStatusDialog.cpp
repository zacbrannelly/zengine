#include "BuildStatusDialog.h"
#include "../Editor.h"
#include "../Project/Project.h"

#include <iostream>

using namespace ZEngine;

BuildStatusDialog::BuildStatusDialog(Editor* editor) : GUIDialog("Building...", 300, 100, true)
{
  _editor = editor;
  SetFlags(ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
  SetDisableClose(true);
  SetVisible(false);
}

void BuildStatusDialog::ProcessInput() {

}

void BuildStatusDialog::RenderElement()
{
  if (_editor->GetProject() == nullptr) return;

  if (_editor->GetProject()->IsBuildInProgress())
  {
    SetVisible(true);
  }
  else if (IsVisible())
  {
    SetVisible(false);
  }

  GUIDialog::RenderElement();
}

void BuildStatusDialog::RenderInWindow()
{
  ImGui::Text("Building Scripts...");
}

BuildStatusDialog::~BuildStatusDialog()
{
}
