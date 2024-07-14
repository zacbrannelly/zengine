#include "GUIWindow.h"

using namespace std;

GUIWindow::GUIWindow(string title, int width, int height, bool isChild) : _title(title), _width(width), _height(height), _isChild(isChild), _shouldSetSize(false), _flags(0)
{
	_closeRequested = false;
	_data = nullptr;
	_shouldFocus = false;
	_isDirty = false;
}

void GUIWindow::Focus()
{
	_shouldFocus = true;
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

void GUIWindow::SetFlags(ImGuiWindowFlags flags)
{
	_flags = flags;
}

ImGuiWindowFlags GUIWindow::GetFlags() const
{
	return _flags;
}

const ImVec2& GUIWindow::GetContentSize() const
{
	return _contentSize;
}

int GUIWindow::GetContentWidth() const
{
	return _contentSize.x;
}

int GUIWindow::GetContentHeight() const
{
	return _contentSize.y;
}

void GUIWindow::SetTitle(std::string title)
{
	_title = title;
}

const std::string& GUIWindow::GetTitle() const
{
	return _title;
}

void GUIWindow::SetCloseRequested(bool requested)
{
	_closeRequested = requested;
}

bool GUIWindow::IsCloseRequested() const
{
	return _closeRequested;
}

void GUIWindow::SetUserData(void* data)
{
	_data = data;
}

void* GUIWindow::GetUserData() const
{
	return _data;
}

void GUIWindow::SetDirty(bool dirty)
{
	_isDirty = dirty;
}

bool GUIWindow::IsDirty() const
{
	return _isDirty;
}

bool GUIWindow::AllowClose()
{
	return true;
}

void GUIWindow::RenderInWindowBeforeElements() {}

void GUIWindow::RenderElement()
{
	bool shouldRender = false;
	bool open = true;

	if (_shouldFocus)
	{
		ImGui::SetNextWindowFocus();
		_shouldFocus = false;
	}

	auto flags = _flags;

	if (_isDirty)
		flags |= ImGuiWindowFlags_UnsavedDocument;

	if (_isChild)
		shouldRender = ImGui::BeginChild(_title.c_str());
	else
		shouldRender = ImGui::Begin(_title.c_str(), &open, flags);

	if (!open && AllowClose())
	{
		_closeRequested = true;
	}

	ImGui::SetWindowSize(_title.c_str(), ImVec2(_width, _height), _shouldSetSize ? ImGuiCond_Always : ImGuiCond_FirstUseEver);
	if (_shouldSetSize)
		_shouldSetSize = false;

	auto windowSize = ImGui::GetWindowSize();
	_width = windowSize.x;
	_height = windowSize.y;

	_contentSize = ImGui::GetContentRegionAvail();

	if (shouldRender)
	{
		RenderInWindowBeforeElements();
		Container::RenderElement();
		RenderInWindow();
		ProcessInput();
	}

	if (_isChild)
		ImGui::EndChild();
	else
		ImGui::End();
}

GUIElementType GUIWindow::GetType()
{
	return GUI_TYPE_WINDOW;
}

GUIWindow::~GUIWindow()
{
}
