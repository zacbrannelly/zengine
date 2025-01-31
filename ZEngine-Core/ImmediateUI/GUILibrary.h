#pragma once

#include "../Misc/Singleton.h"

namespace ZEngine
{
	class Display;

	class GUILibrary : public Singleton<GUILibrary>
	{
	public:
		void Init(Display* display);
		void Shutdown();

		void NewFrame();
		void EndFrame();

		void BeginToolbarWindow();
		void SetToolbarWindowHeight(float toolbarHeight);
		void EndToolbarWindow();

		void BeginMainWindow();
		void EndMainWindow();

	private:
		float _toolbarHeight;
	};
}
