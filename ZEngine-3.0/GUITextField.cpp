#include "GUITextField.h"
#include "imgui-includes.h"

GUITextField::GUITextField(std::string label)
{
	_label = label;
	_buffer = new char[GUITEXTFIELD_BUFFER_SIZE];
	_bufferSize = GUITEXTFIELD_BUFFER_SIZE;

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

void GUITextField::RenderElement()
{
	ImGui::InputText(_label.c_str(), _buffer, _bufferSize);
}

GUIElementType GUITextField::GetType()
{
	return GUI_TYPE_TEXTFIELD;
}

GUITextField::~GUITextField()
{
	delete[] _buffer;
}
