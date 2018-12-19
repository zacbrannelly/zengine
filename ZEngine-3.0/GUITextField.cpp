#include "GUITextField.h"
#include "imgui-includes.h"

GUITextField::GUITextField(std::string label)
{
	_label = label;
	_buffer = new char[GUITEXTFIELD_BUFFER_SIZE];
	_bufferSize = GUITEXTFIELD_BUFFER_SIZE;
	_isMultiline = false;
	_size = ImVec2(0, 0);

	memset(_buffer, '\0', _bufferSize);
}

void GUITextField::SetLabel(std::string label)
{
	_label = label;
}

std::string GUITextField::GetLabel() const
{
	return _label;
}

void GUITextField::SetText(std::string text)
{
	memset(_buffer, '\0', _bufferSize);
	memcpy(_buffer, &text[0], text.size());
}

std::string GUITextField::GetText() const
{
	return std::string(_buffer);
}

void GUITextField::SetBufferSize(int size)
{
	if (size != _bufferSize)
	{
		delete[] _buffer;

		_buffer = new char[size];
		memset(_buffer, '\0', size);
	}

	_bufferSize = size;
}

int GUITextField::GetButterSize() const
{
	return _bufferSize;
}

void GUITextField::SetMultiline(bool isMulti)
{
	_isMultiline = isMulti;

	if (_isMultiline &&_bufferSize < 1000000)
	{
		SetBufferSize(1000000);
	}
}

void GUITextField::SetSize(float width, float height)
{
	_size = { width, height };
}

float GUITextField::GetWidth() const
{
	return _size.x;
}

float GUITextField::GetHeight() const
{
	return _size.y;
}

bool GUITextField::IsMultiline() const
{
	return _isMultiline;
}

void GUITextField::RenderElement()
{
	if (!_isMultiline)
		ImGui::InputText(_label.c_str(), _buffer, _bufferSize);
	else
		ImGui::InputTextMultiline(_label.c_str(), _buffer, _bufferSize, _size, ImGuiInputTextFlags_AllowTabInput);
}

GUIElementType GUITextField::GetType()
{
	return GUI_TYPE_TEXTFIELD;
}

GUITextField::~GUITextField()
{
	delete[] _buffer;
}
