#include "LogWindow.h"

using namespace ZEngine;


LogWindow::LogWindow() : GUIWindow("Log", 600, 150, false)
{
  _loggingSystem = LoggingSystem::GetInstance();
}

void LogWindow::ProcessInput()
{

}

ImVec4 LogLevelToColor(LogLevel level) {
  switch (level)
  {
    case LOG_ERROR: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Red
    case LOG_WARNING: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow
    case LOG_DEBUG: return ImVec4(0.5f, 0.5f, 1.0f, 1.0f); // Blue
    case LOG_INFO:
    default: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White
  }
}

void LogWindow::RenderInWindow()
{
  auto logs = _loggingSystem->GetLogs();

  ImGui::BeginTable("LogTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable);
  ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_WidthFixed, 100.0f);
  ImGui::TableSetupColumn("Module", ImGuiTableColumnFlags_WidthFixed, 200.0f);
  ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
  ImGui::TableHeadersRow();

  for (int i = 0; i < logs.size(); i++)
  {
    // Reverse order for most recent logs at the top
    const auto& log = logs[logs.size() - 1 - i];

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::TextColored(LogLevelToColor(log.level), "%s", LoggingSystem::LogLevelToString(log.level).c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", log.module.c_str());
    ImGui::TableNextColumn();
    ImGui::Text("%s", log.message.c_str());
  }

  ImGui::EndTable();
}

LogWindow::~LogWindow()
{
}
