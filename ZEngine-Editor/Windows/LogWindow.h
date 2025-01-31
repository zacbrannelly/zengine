#pragma once

#include <ZEngine-Core/ImmediateUI/GUIWindow.h>

namespace ZEngine
{
	class LogWindow : public GUIWindow
	{
	public:
		LogWindow();
		~LogWindow();

		void ProcessInput() override;
		void RenderInWindow() override;
	};
}
