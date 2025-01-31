#pragma once

#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
#include <GLFW/glfw3.h>
#endif

#include <string>

namespace ZEngine
{
	class Display
	{
	public:
		Display(std::string title, int width, int height);
		~Display();

		bool Init();
		void Update();
		void Shutdown();

	#if !TARGET_OS_IPHONE
		GLFWwindow* GetHandle() const;
	#endif

		// TODO: Should we strip this out for iOS too?
		void* GetNativeHandle() const;

		void SetTitle(std::string title);
		std::string GetTitle() const;

		void SetSize(int width, int height);

		void SetWidth(int width);
		int GetWidth() const;

		void SetHeight(int height);
		int GetHeight() const;

		void SetDecorated(bool isDecorated);
		bool IsDecorated() const;

		void SetResizable(bool isResizable);
		bool IsResizable() const;

		void SetVisible(bool isVisible);
		bool IsVisible() const;

		void SetFullscreen(bool isFullscreen);
		bool IsFullscreen() const;

		void SetMaximizeOnCreation(bool willMaximize);
		bool WillMaximizeOnCreation() const;

		void RequestClose() const;
		bool CloseRequested() const;
		bool IsInitialized() const;
	private:
		int _width, _height;
		std::string _title;

	#if !TARGET_OS_IPHONE
		GLFWwindow* _handle;
		static void CallbackWindowResize(GLFWwindow* handle, int newWidth, int newHeight);
	#endif

		bool _isInitialized;
		bool _isDecorated;
		bool _isResizable;
		bool _isVisible;
		bool _isFullscreen;
		bool _maximize;
	};
}
