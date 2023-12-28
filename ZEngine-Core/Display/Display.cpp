#include "Display.h"
#include "../Rendering/Graphics.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#define GLFW_EXPOSE_NATIVE_WIN32

#else

#define GLFW_EXPOSE_NATIVE_COCOA

#endif

#if !TARGET_OS_IPHONE
#include <GLFW/glfw3native.h>
#endif

#include <iostream>

Display::Display(std::string title, int width, int height) : _title(title), _width(width), _height(height)
{
#if !TARGET_OS_IPHONE
	_handle = nullptr;
#endif
	_isInitialized = false;
	_isFullscreen = false;
	_isDecorated = true;
	_isResizable = true;
	_isVisible = true;
	_maximize = false;
}

bool Display::Init()
{
#if !TARGET_OS_IPHONE
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "DISPLAY: Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_DECORATED, _isDecorated ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, _isResizable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, _isVisible ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_MAXIMIZED, _maximize ? GLFW_TRUE : GLFW_FALSE);

	// TODO: Figure out if this is doing anything.
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

	GLFWmonitor* monitor = nullptr;

	if (_isFullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
	}

	_handle = glfwCreateWindow(_width, _height, _title.c_str(), monitor, nullptr);

	if (_handle == nullptr)
	{
		std::cout << "DISPLAY: Failed to create window" << std::endl;
		return false;
	}

	glfwSetWindowUserPointer(_handle, this);
	glfwSetWindowSizeCallback(_handle, &Display::CallbackWindowResize);
#endif

	_isInitialized = true;

	return true;
}

void Display::Update()
{
#if !TARGET_OS_IPHONE
	glfwPollEvents();
#endif
}

void Display::RequestClose() const
{
#if !TARGET_OS_IPHONE
	if (_handle == nullptr) return;
	glfwSetWindowShouldClose(_handle, GLFW_TRUE);
#endif
}

bool Display::CloseRequested() const
{
#if !TARGET_OS_IPHONE
	return _handle != nullptr ? glfwWindowShouldClose(_handle) : false;
#else
	return false;
#endif
}

bool Display::IsInitialized() const
{
	return _isInitialized;
}

void Display::Shutdown()
{
#if !TARGET_OS_IPHONE
	glfwDestroyWindow(_handle);
	glfwTerminate();
#endif
}

void Display::SetTitle(std::string title)
{
	_title = title;

#if !TARGET_OS_IPHONE
	if (_handle != nullptr)
		glfwSetWindowTitle(_handle, _title.c_str());
#endif
}

std::string Display::GetTitle() const
{
	return _title;
}

void Display::SetSize(int width, int height)
{
	_width = width;
	_height = height;

#if !TARGET_OS_IPHONE
	if (_handle != nullptr)
		glfwSetWindowSize(_handle, _width, _height);
#endif
}

void Display::SetWidth(int width)
{
	_width = width;
	SetSize(_width, _height);
}	

int Display::GetWidth() const
{
	return _width;
}

void Display::SetHeight(int height)
{
	_height = height;
	SetSize(_width, _height);
}

int Display::GetHeight() const
{
	return _height;
}

void Display::SetFullscreen(bool isFullscreen)
{
#if !TARGET_OS_IPHONE
	if (_isInitialized)
	{
		if (isFullscreen && !_isFullscreen)
			glfwSetWindowMonitor(_handle, glfwGetPrimaryMonitor(), 0, 0, _width, _height, GLFW_DONT_CARE);
		else if (!isFullscreen && _isFullscreen)
			glfwSetWindowMonitor(_handle, nullptr, 0, 0, _width, _height, GLFW_DONT_CARE);
	}
#endif

	_isFullscreen = isFullscreen;
}

bool Display::IsFullscreen() const
{
	return _isFullscreen;
}

void Display::SetDecorated(bool isDecorated)
{
	_isDecorated = isDecorated;
}

bool Display::IsDecorated() const
{
	return _isDecorated;
}

void Display::SetVisible(bool isVisible)
{
#if !TARGET_OS_IPHONE
	if (_isInitialized)
	{
		if (isVisible && !_isVisible)
			glfwShowWindow(_handle);
		else if (!isVisible && _isVisible)
			glfwHideWindow(_handle);
	}
#endif

	_isVisible = isVisible;
}

bool Display::IsVisible() const
{
	return _isVisible;
}

void Display::SetResizable(bool isResizable)
{
	_isResizable = isResizable;
}

bool Display::IsResizable() const
{
	return _isResizable;
}

void Display::SetMaximizeOnCreation(bool willMaximize)
{
	_maximize = willMaximize;
}

bool Display::WillMaximizeOnCreation() const
{
	return _maximize;
}

void* Display::GetNativeHandle() const
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	return glfwGetWin32Window(_handle);
#elif !TARGET_OS_IPHONE
  // TODO: Get native handle for other platforms
	return glfwGetCocoaWindow(_handle);
#else
  // TODO: Figure out how to get native handle on iOS (if we need it).
	return nullptr;
#endif
}

#if !TARGET_OS_IPHONE
GLFWwindow* Display::GetHandle() const
{
	return _handle;
}

void Display::CallbackWindowResize(GLFWwindow* window, int newWidth, int newHeight)
{
	auto data = glfwGetWindowUserPointer(window);

	if (data != nullptr)
	{
		auto display = static_cast<Display*>(data);

		display->_width = newWidth;
		display->_height = newHeight;

		auto graphics = Graphics::GetInstance();

		if (graphics->IsInitialized())
			graphics->Reset(newWidth, newHeight, 0);
	}
}
#endif

Display::~Display()
{
}
