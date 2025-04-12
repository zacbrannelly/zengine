#pragma once

#include <ZEngine-Core/ImmediateUI/GUIWindow.h>
#include <ZEngine-Core/Logging/LoggingSystem.h>

namespace ZEngine
{
	class LogWindow : public GUIWindow
	{
	public:
		LogWindow();
		~LogWindow();

		void ProcessInput() override;
		void RenderInWindow() override;
	private:
		LoggingSystem* _loggingSystem;
	};
}
