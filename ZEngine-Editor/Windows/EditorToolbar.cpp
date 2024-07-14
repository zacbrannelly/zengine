#include "EditorToolbar.h"
#include "../Editor.h"
#include "../Menus/MainMenuBar.h"
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>
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
  auto playButtonLabel = ICON_FA_PLAY_CIRCLE "  Play";
  ImVec2 buttonSize(ImGui::CalcTextSize(playButtonLabel).x + 20, 0);

  ImGui::SetCursorPosX(windowWidth / 2 - buttonSize.x / 2);
  if (ImGui::Button(playButtonLabel, buttonSize)) {
    _mapController->Play();
  }
}

void EditorToolbar::RenderPauseAndStopButtons()
{
  auto windowWidth = ImGui::GetWindowContentRegionWidth();
  auto isPaused = _mapController->GetPlayState() == PAUSED;
  auto pauseToggleLabel = isPaused ? (ICON_FA_PLAY_CIRCLE "  Continue") : (ICON_FA_PAUSE_CIRCLE "  Pause");
  auto stopButtonLabel = ICON_FA_STOP "  Stop";

  ImVec2 pauseButtonSize(ImGui::CalcTextSize(pauseToggleLabel).x + 20, 0);
  ImVec2 stopButtonSize(ImGui::CalcTextSize(stopButtonLabel).x + 20, 0);

  ImGui::SetCursorPosX(windowWidth / 2 - pauseButtonSize.x - 5);
  if (ImGui::Button(pauseToggleLabel, pauseButtonSize)) {
    isPaused 
      ? _mapController->Continue() 
      : _mapController->Pause();
  }

  ImGui::SameLine();
  
  ImGui::SetCursorPosX(windowWidth / 2 + 5);
  if (ImGui::Button(stopButtonLabel, stopButtonSize)) {
    _mapController->Stop();
  }
}

EditorToolbar::~EditorToolbar()
{
}
