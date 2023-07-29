#include "GUILibrary.h"

#include "../imgui-includes.h"
#include <ZEngine-Core/Display/Display.h>
#include <ZEngine-Core/Input/InputManager.h>
#include <ZEngine-Core/Rendering/Graphics.h>

void GUILibrary::Init(Display* display)
{
	imguiCreate();
	ImGui_ImplGlfw_InitForOpenGL(display->GetHandle(), false);
	
	auto input = InputManager::GetInstance();

	// Register the callbacks to the input manager
	input->RegisterMouseButtonCallback(ImGui_ImplGlfw_MouseButtonCallback);
	input->RegisterKeyCallback(ImGui_ImplGlfw_KeyCallback);

	// Setup the rest of the callbacks we don't clash with (yet)
	glfwSetScrollCallback(display->GetHandle(), ImGui_ImplGlfw_ScrollCallback);
	glfwSetCharCallback(display->GetHandle(), ImGui_ImplGlfw_CharCallback);

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();
	ImGui::LoadIniSettingsFromDisk("layout.ini");
}

void GUILibrary::NewFrame()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Get the display size from ImGui
	auto displaySize = ImGui::GetIO().DisplaySize;

	// Create a dockspace window that is the size of the display always 
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(displaySize);
	ImGui::Begin("MainWindow", (bool*)0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDecoration);
	ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
}

void GUILibrary::EndFrame()
{
	ImGui::GetIO().DisplayFramebufferScale = ImVec2(1, 1);

	ImGui::End();
	ImGui::EndFrame();
	imguiEndFrame();
}

void GUILibrary::Shutdown()
{
	ImGui::SaveIniSettingsToDisk("layout.ini");
	imguiDestroy();
}