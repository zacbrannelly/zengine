#include <algorithm>
#include <iostream>

#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Utilities/Directory.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/AssetManager.h>

#include "CreateProjectDialog.h"
#include "BrowserDialog.h"
#include "../UI/GUITextField.h"
#include "../Builders/ProjectBuilder.h"
#include "../Editor.h"


CreateProjectDialog::CreateProjectDialog(Editor* editor, std::string basePath) : GUIDialog("New Project", 500, 200, true)
{
  _editor = editor;
  _basePath = Directory::GetAbsolutePath(basePath);

  _nameField = new GUITextField("Name");
  _pathField = new GUITextField("Path");
  _browser = new BrowserDialog(basePath, BROWSER_OPEN_FOLDER);

  SetVisible(true);
  SetFlags(ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

}

bool CreateProjectDialog::Validate()
{
  auto name = _nameField->GetText();

  if (name == "")
    return false;

  if (std::any_of(name.begin(), name.end(), [](const auto& c) { return c == ' '; }))
    return false;

  auto path = _pathField->GetText();

  if (path == "")
    return false;

  if (std::any_of(path.begin(), path.end(), [](const auto& c) { return c == ' '; }))
    return false;
  
  return true;
}

bool CreateProjectDialog::Create()
{
  if (!Validate()) return false;

  auto name = _nameField->GetText();
  auto path = _pathField->GetText();

  // Generate the project
  Project* project = nullptr;
  try {
    project = ProjectBuilder::CreateProject(name, path);
  }
  catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return false;
  }

  // Load the project into the editor.
  _editor->SetProject(project);

  return true;
}

void CreateProjectDialog::ProcessInput()
{
  if (GetResult() == DIALOG_RESULT_CLOSE)
    Close();

  if (_browser->IsVisible())
  {
    if (_browser->GetResult() == DIALOG_RESULT_OK)
    {
      _pathField->SetText(_browser->GetSelectedDirectory().GetAbsolutePath());
      _basePath = _browser->GetSelectedDirectory().GetAbsolutePath();

      _browser->Hide();
    }
    else if (_browser->GetResult() == DIALOG_RESULT_CLOSE)
      _browser->Hide();
  }

  if (_nameField->GetText() == "")
    _pathField->SetText(_basePath);
}

void CreateProjectDialog::RenderInWindow()
{
  // Line 1: Name field
  _nameField->RenderElement();

  // Line 2: Path field with browser button
  if (ImGui::Button("..."))
  {
    _browser->Show();
  }
  _browser->RenderElement();

  ImGui::SameLine();

  _pathField->RenderElement();

  // Line 3: Create and Cancel buttons
  ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - 80 * 2 - 8);
  ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMax().y - 8 - 28);

  if (ImGui::Button("Cancel", ImVec2(80, 0)))
    Close();

  ImGui::SameLine();

  if (ImGui::Button("Create", ImVec2(80, 0)))
  {
    if (Create())
      Close();
  }
}

void CreateProjectDialog::RenderElement()
{
  GUIDialog::RenderElement();
}

CreateProjectDialog::~CreateProjectDialog()
{
  delete _nameField;
  delete _pathField;
  delete _browser;
}