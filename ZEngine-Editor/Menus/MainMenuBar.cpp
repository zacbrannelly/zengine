#include "MainMenuBar.h"
#include "../Editor.h"
#include "../Dialogs/CreateProjectDialog.h"
#include "../Dialogs/ProjectBrowserDialog.h"
#include "../imgui-includes.h"

MainMenuBar::MainMenuBar(Editor* editor) : _editor(editor)
{
}

void MainMenuBar::RenderElement()
{
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
		ImGui::MenuItem("New Map", NULL, (bool*)NULL);
		ImGui::MenuItem("Open Map", NULL, (bool*)NULL);
		ImGui::MenuItem("Save Map", NULL, (bool*)NULL);
		ImGui::MenuItem("Close", NULL, (bool*)NULL);

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
