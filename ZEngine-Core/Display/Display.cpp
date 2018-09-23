#include "Display.h"
#include "../Rendering/Graphics.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW\glfw3native.h>
#include <iostream>

Display::Display(std::string title, int width, int height) : _title(title), _width(width), _height(height), _handle(nullptr)
{

}

bool Display::Init()
{
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "DISPLAY: Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	_handle = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);

	if (_handle == nullptr)
	{
		std::cout << "DISPLAY: Failed to create window" << std::endl;
		return false;
	}

	glfwSetWindowUserPointer(_handle, this);
	glfwSetWindowSizeCallback(_handle, &Display::CallbackWindowResize);

	return true;
}

void Display::Update()
{
	glfwPollEvents();
}

bool Display::CloseRequested() const
{
	return _handle != nullptr ? glfwWindowShouldClose(_handle) : false;
}

void Display::Shutdown()
{
	glfwDestroyWindow(_handle);
	glfwTerminate();
}

void Display::GrabMouse() const
{
	glfwSetInputMode(_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Display::SetTitle(std::string title)
{
	_title = title;

	if (_handle != nullptr)
		glfwSetWindowTitle(_handle, _title.c_str());
}

std::string Display::GetTitle() const
{
	return _title;
}

void Display::SetSize(int width, int height)
{
	if (_handle != nullptr)
		glfwSetWindowSize(_handle, _width, _height);
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

void* Display::GetWin32Handle() const
{
	return glfwGetWin32Window(_handle);
}

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

Display::~Display()
{
}
