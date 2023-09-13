#include "EditorToolbar.h"
#include "../Editor.h"
#include "../Menus/MainMenuBar.h"
#include "../imgui-includes.h"
#include "../Controllers/MapController.h"

EditorToolbar::EditorToolbar(Editor* editor) : _editor(editor)
{
  _mapController = editor->GetMapController();
  Add(new MainMenuBar(editor));
}

void EditorToolbar::RenderElement() 
{
  Container::RenderElement();

  switch (_mapController->GetPlayState())
  {
    case STOPPED:
      RenderPlayButton();
      break;
    default:
      RenderPauseAndStopButtons();
      break;
  }
}

void EditorToolbar::RenderPlayButton()
{
  auto windowWidth = ImGui::GetWindowContentRegionWidth();
  ImVec2 buttonSize(ImGui::CalcTextSize("Build & Run").x + 20, 0);

  ImGui::SetCursorPosX(windowWidth / 2 - buttonSize.x / 2);
  if (ImGui::Button("Build & Run", buttonSize)) {
    _mapController->Play();
  }
}

void EditorToolbar::RenderPauseAndStopButtons()
{
  auto windowWidth = ImGui::GetWindowContentRegionWidth();
  auto isPaused = _mapController->GetPlayState() == PAUSED;
  auto pauseToggleLabel = isPaused ? "Continue" : "Pause";

  ImVec2 pauseButtonSize(ImGui::CalcTextSize(pauseToggleLabel).x + 20, 0);
  ImVec2 stopButtonSize(ImGui::CalcTextSize("Stop").x + 20, 0);

  ImGui::SetCursorPosX(windowWidth / 2 - pauseButtonSize.x - 5);
  if (ImGui::Button(pauseToggleLabel, pauseButtonSize)) {
    isPaused 
      ? _mapController->Continue() 
      : _mapController->Pause();
  }

  ImGui::SameLine();
  
  ImGui::SetCursorPosX(windowWidth / 2 + 5);
  if (ImGui::Button("Stop", stopButtonSize)) {
    _mapController->Stop();
  }
}

EditorToolbar::~EditorToolbar()
{
}
