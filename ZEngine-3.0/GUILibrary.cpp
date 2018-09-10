#include "GUILibrary.h"

#include "imgui-includes.h"
#include <ZEngine-Core\Display\Display.h>

void GUILibrary::Init(Display* display)
{
	imguiCreate();
	ImGui_ImplGlfw_InitForOpenGL(display->GetHandle(), true);
	ImGui::StyleColorsDark();
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
	imguiDestroy();
}