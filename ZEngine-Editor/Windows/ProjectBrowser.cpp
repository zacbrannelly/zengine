#include "ProjectBrowser.h"

#include "../Editor.h"
#include "../Project/Project.h"
#include "../Dialogs/ImportDialog.h"
#include "../Dialogs/CreateMaterialDialog.h"
#include "../Dialogs/CreateMapDialog.h"
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>
#include <ZEngine-Core/Assets/Objects/MapAsset.h>
#include <ZEngine-Core/Assets/Objects/TextureAsset.h>
#include <imgui_internal.h>

#include <iostream>

using namespace ZEngine;

constexpr auto ICON_SIZE = 80;

ProjectBrowser::ProjectBrowser(Editor* editor) : GUIWindow("Browser", 600, 150, false), _currentDirectory("")
{
  _editor = editor;
  _selectedItem = 0;
  _assetManager = AssetManager::GetInstance();
}

void ProjectBrowser::ProcessInput()
{

}

bool ProjectBrowser::RenderFile(
  const std::string& basename,
  bool selected,
  bool isDirectory,
  bool isImported,
  ImTextureID previewTextureId
) {
  ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

  std::string icon;
  if (basename == "..") 
  {
    icon = ICON_FA_LEVEL_UP;
  }
  else
  {
    icon = isDirectory ? ICON_FA_FOLDER : ICON_FA_FILE;
  }

  auto pressed = ImGui::Selectable(
    icon.c_str(),
    selected,
    ImGuiSelectableFlags_AllowDoubleClick,
    ImVec2(ICON_SIZE, ICON_SIZE)
  );
  ImGui::PopStyleVar();

  auto itemMin = ImGui::GetItemRectMin();
  auto itemMax = ImGui::GetItemRectMax();

  auto drawList = ImGui::GetWindowDrawList();

  // Render consistent background for all items.
  drawList->AddRectFilled(itemMin, itemMax, IM_COL32(255, 255, 255, 30));

  // Render question mark on non-imported files.
  if (!isDirectory && !isImported)
  {
    drawList->AddText(ImVec2(itemMin.x + 8, itemMin.y + 4), IM_COL32(255, 255, 255, 255), ICON_FA_QUESTION_CIRCLE);
  }

  auto textMin = ImVec2(itemMin.x, itemMax.y);
  auto textMax = ImVec2(itemMax.x, itemMax.y + 18);
  ImGui::RenderTextClipped(textMin, textMax, basename.c_str(), NULL, NULL);

  // Render preview image.
  if (previewTextureId > 0)
  {
    drawList->AddImage(
      previewTextureId,
      itemMin,
      itemMax,
      ImVec2(0, 0),
      ImVec2(1, 1)
    );

    // Render hover effect.
    if (ImGui::IsItemHovered())
    {
      drawList->AddRectFilled(itemMin, itemMax, IM_COL32(255, 255, 255, 50));
    }
  }

  if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
  {
    std::string tooltip(basename);

    if (!isImported && !isDirectory)
    {
      tooltip += " (Not Imported)";
    }

    if (basename == "..")
    {
      tooltip = "Go up a directory";
    }

    ImGui::BeginTooltip();
    ImGui::Text(tooltip.c_str());
    ImGui::EndTooltip();
  }

  return pressed;
}

void ProjectBrowser::RenderDirectoryContextMenu()
{
  // TODO
  ImGui::Text("Directory context menu...");
}

void ProjectBrowser::RenderFileContextMenu(File const& file, bool isImported)
{
  if (!isImported && ImGui::MenuItem("Add to project"))
  {
    ObjectType type;
    if (GetObjectTypeFromExtension(file.GetExtension(), type)) {
      _viewingProject->GetCatalog().RegisterAsset(file.GetPath(), type);
      _viewingProject->Save();
    }
  }
}

void ProjectBrowser::RenderGeneralContextMenu()
{
  if (ImGui::BeginMenu("Create"))
  {
    if (ImGui::MenuItem("Map"))
    {
      auto dialog = new CreateMapDialog(_editor, _currentDirectory);
      dialog->Show();
      _editor->Add(dialog);
    }

    if (ImGui::MenuItem("Material"))
    {
      auto dialog = new CreateMaterialDialog(_editor, _currentDirectory);
      dialog->Show();
      _editor->Add(dialog);
    }

    if (ImGui::MenuItem("Script"))
    {
      std::cout << "Creating new script..." << std::endl;
    }

    if (ImGui::MenuItem("Shader"))
    {
      std::cout << "Creating new shader..." << std::endl;
    }

    ImGui::EndMenu();
  }

  ImGui::Separator();

  if (ImGui::MenuItem("Import..."))
  {
    _editor->Add(new ImportDialog(_editor, Directory(_currentDirectory)));
  }
}

void ProjectBrowser::NavigateTo(const std::string& path)
{
  _files.clear();
  _directories.clear();
  if (_viewingProject == nullptr) return;

  auto directory = GetCurrentDirectory();

  if (path == "..")
  {
    directory = directory.GetParentDirectory();
  }
  else if (path != ".")
  {
    directory = directory.FindDirectory(path);
  }

  auto isRoot = directory.GetPath() == _viewingProject->GetProjectDirectory().GetPath();

  if (directory.Exists())
  {
    _currentDirectory = directory.GetPath();

    auto files = directory.GetAllFiles();
    for (auto& file : files)
    {
      // Ignore .DS_Store files
      auto name = file.GetFilename();
      if (name == ".DS_Store") continue;

      _files.push_back(file);
    }

    auto directories = directory.GetAllDirectories();
    for (auto& dir : directories)
    {
      auto name = dir.GetName();

      // Always show . regardless of where we are
      if (name == ".") continue;

      // Ignore .. if we are not at the root
      if (isRoot && name == "..") continue;

      _directories.push_back(dir);
    }
  }
}

void ProjectBrowser::OpenFile(const std::string& path)
{
  auto currentDirectory = GetCurrentDirectory();
  auto file = currentDirectory.FindFile(path);

  if (file.GetExtension() == "map")
  {
    std::cout << "Opening map: " << file.GetPath() << std::endl;
    auto mapAsset = _assetManager->FindAssetFromPath(file.GetPath());
    if (mapAsset == nullptr)
    {
      mapAsset = _assetManager->LoadAsset(file.GetPath(), MAP_ASSET);
    }

    if (mapAsset != nullptr)
    {
      _editor->SetSelectedMapAsset(mapAsset->Cast<MapAsset>());
    }
  }
}

void ProjectBrowser::RenderInWindow()
{
  auto currentProject = _editor->GetProject();
  if (currentProject != _viewingProject)
  {
    _viewingProject = currentProject;
    _selectedItem = 0;
    _currentDirectory = "";
    NavigateTo(".");
  }

  int spacing = 8;
  int itemsPerRow = floor((float)GetContentWidth() / (ICON_SIZE + spacing * 2));

  std::string label;
  bool isDirectory = false;
  int contextMenuId = -1;

  for (int i = 0; i < _files.size() + _directories.size(); i++)
  {
    if (i < _directories.size())
    {
      auto dir = _directories[i];
      label = dir.GetName();
      isDirectory = true;
    }
    else
    {
      auto file = _files[i - _directories.size()];
      label = file.GetFilename();
      isDirectory = false;
    }

    if (i % itemsPerRow != 0)
    {
      ImGui::SameLine();
      ImGui::SetCursorPosX(ImGui::GetCursorPosX() + spacing);
    }
    else if (i >= itemsPerRow)
    {
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + spacing + 18);
    }

    bool isImported = false;

    ImTextureID previewTextureId = 0;
    if (!isDirectory)
    {
      // Check if the file is imported into the project.
      auto file = _files[i - _directories.size()];
      isImported = _viewingProject->GetCatalog().HasAsset(file.GetPath());

      // Determine type of asset, create preview if applicable.
      ObjectType type;
      if (_viewingProject->GetCatalog().GetAssetTypeFromPath(file.GetPath(), type))
      {
        if (type == TEXTURE_ASSET)
        {
          auto textureAsset = AssetManager::GetInstance()->LoadAsset(file.GetPath(), TEXTURE_ASSET)->Cast<TextureAsset>();
          auto textureId = textureAsset->GetTexture()->GetHandle().idx;
          if (textureId != bgfx::kInvalidHandle)
          {
            previewTextureId = (ImTextureID)textureId;
          }
        }
      }
    }

    ImGui::PushID(i);

    if (RenderFile(label, i == _selectedItem, isDirectory, isImported, previewTextureId))
    {
      _selectedItem = i;
      if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
      {
        if (isDirectory)
        {
          NavigateTo(label);
        }
        else 
        {
          OpenFile(label);
        }
      }
    }

    // Trigger context menu for this item.
    if (ImGui::BeginPopupContextItem())
    {
      contextMenuId = i;
      isDirectory 
        ? RenderDirectoryContextMenu() 
        : RenderFileContextMenu(_files[i - _directories.size()], isImported);
      ImGui::EndPopup();
    }

    ImGui::PopID();
  }

  // Add invisible button to fill the last row with empty space.
  ImGui::InvisibleButton("##spacer", ImVec2(ICON_SIZE, ICON_SIZE));

  // Trigger context menu for empty space and no item's context menu was triggered.
  if (contextMenuId < 0 && ImGui::BeginPopupContextWindow())
  {
    RenderGeneralContextMenu();
    ImGui::EndPopup();
  }
}

void ProjectBrowser::RenderElement()
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
  GUIWindow::RenderElement();
  ImGui::PopStyleVar();
}

Directory ProjectBrowser::GetCurrentDirectory() const
{
  return _currentDirectory.empty() 
    ? _viewingProject->GetProjectDirectory() 
    : Directory(_currentDirectory);
}

ProjectBrowser::~ProjectBrowser()
{
}
