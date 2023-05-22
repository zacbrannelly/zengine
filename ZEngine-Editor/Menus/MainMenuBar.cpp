#include "MainMenuBar.h"
#include "../imgui-includes.h"

MainMenuBar::MainMenuBar()
{
}

void MainMenuBar::RenderElement()
{
	ImGui::BeginMenuBar();

	if (ImGui::BeginMenu("File"))
	{
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
