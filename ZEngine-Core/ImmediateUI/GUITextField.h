#pragma once

#include <string>
#include "GUIElement.h"
#include "imgui-includes.h"

#define GUITEXTFIELD_BUFFER_SIZE 1000

namespace ZEngine
{
	class GUITextField : public GUIElement
	{
	public:
		GUITextField(std::string label);
		~GUITextField();

		void SetLabel(std::string label);
		std::string GetLabel() const;

		void SetText(std::string text);
		std::string GetText() const;

		void SetBufferSize(int size);
		int GetButterSize() const;

		void SetMultiline(bool isMulti);
		bool IsMultiline() const;

		void SetSize(float width, float height);
		float GetWidth() const;
		float GetHeight() const;

		void RenderElement() override;
		GUIElementType GetType();

	private:
		std::string _label;
		char* _buffer;
		int _bufferSize;
		bool _isMultiline;
		ImVec2 _size;
	};
}
