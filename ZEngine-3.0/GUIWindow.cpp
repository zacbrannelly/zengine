#include "GUIWindow.h"
#include "imgui-includes.h"

using namespace std;

GUIWindow::GUIWindow(string title, int width, int height, bool isChild) : _title(title), _width(width), _height(height), _isChild(isChild), _shouldSetSize(false)
{
}

void GUIWindow::SetSize(int width, int height)
{
	_width = width;
	_height = height;
	_shouldSetSize = true;
}

int GUIWindow::GetWidth() const
{
	return _width;
}

int GUIWindow::GetHeight() const
{
	return _height;
}

void GUIWindow::RenderElement()
{
	if (_isChild)
		ImGui::BeginChild(_title.c_str());
	else
		ImGui::Begin(_title.c_str());

	ImGui::SetWindowSize(_title.c_str(), ImVec2(_width, _height), _shouldSetSize ? ImGuiCond_Always : ImGuiCond_Once);
	if (_shouldSetSize)
		_shouldSetSize = false;

	auto windowSize = ImGui::GetWindowSize();
	_width = windowSize.x;
	_height = windowSize.y;

	Container::RenderElement();
	ProcessInput();

	if (_isChild)
		ImGui::EndChild();
	else
		ImGui::End();
}

GUIWindow::~GUIWindow()
{
}
