#include "MainMenuBar.h"
#include "../Editor.h"
#include "../Dialogs/CreateProjectDialog.h"
#include "../Dialogs/ProjectBrowserDialog.h"
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

using namespace ZEngine;

MainMenuBar::MainMenuBar(Editor* editor) : _editor(editor)
{
}

void MainMenuBar::RenderElement()
{
	// Check if the user pressed CMD + S, if so, save the selected map asset
	if (ImGui::IsKeyChordPressed(ImGuiKey_S | ImGuiMod_Ctrl))
	{
		_editor->SaveSelectedMapAsset();
	}

	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Project", NULL, (bool*)NULL))
		{
			_editor->Add(new CreateProjectDialog(_editor));
		}
		if (ImGui::MenuItem("Open Project", NULL, (bool*)NULL))
		{
			_editor->Add(new ProjectBrowserDialog(_editor));
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Save Map", "CMD + S", (bool*)NULL))
		{
			_editor->SaveSelectedMapAsset();
		}

		ImGui::Separator();

		if (ImGui::BeginMenu("Layout")) {
			if (ImGui::MenuItem("Load Layout", NULL, (bool*)NULL))
			{
#ifdef __EMSCRIPTEN__
				ImGui::LoadIniSettingsFromDisk("/disk/layout.ini");
#else
				ImGui::LoadIniSettingsFromDisk("layout.ini");
#endif
			}
			if (ImGui::MenuItem("Save Layout", NULL, (bool*)NULL))
			{
#ifdef __EMSCRIPTEN__
				ImGui::SaveIniSettingsToDisk("/disk/layout.ini");
#else
				ImGui::SaveIniSettingsToDisk("layout.ini");
#endif
			}
			ImGui::EndMenu();
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Close", NULL, (bool*)NULL))
		{
			_editor->RequestClose();
		}

		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Edit"))
	{
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Window"))
	{
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("About"))
	{
		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
}

GUIElementType MainMenuBar::GetType()
{
	return GUI_TYPE_MAIN_MENU_BAR;
}

MainMenuBar::~MainMenuBar()
{
}
