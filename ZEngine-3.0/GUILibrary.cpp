#include "GUILibrary.h"

#include "imgui-includes.h"
#include <ZEngine-Core\Display\Display.h>
#include <ZEngine-Core\Input\InputManager.h>

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

	ImGui::StyleColorsDark();
	ImGui::LoadIniSettingsFromDisk("layout.ini");
}

void GUILibrary::NewFrame()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void GUILibrary::EndFrame()
{
	ImGui::EndFrame();
	imguiEndFrame();
}

void GUILibrary::Shutdown()
{
	ImGui::SaveIniSettingsToDisk("layout.ini");
	imguiDestroy();
}