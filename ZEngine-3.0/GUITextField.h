#pragma once

#include <string>
#include "GUIElement.h"

#define GUITEXTFIELD_BUFFER_SIZE 1000

class GUITextField : public GUIElement
{
public:
	GUITextField(std::string label);
	~GUITextField();

	void SetLabel(std::string label);
	std::string GetLabel() const;

	void SetText(std::string text);
	std::string GetText() const;

	void RenderElement() override;
	GUIElementType GetType();

private:
	std::string _label;
	char* _buffer;
	int _bufferSize;
};
