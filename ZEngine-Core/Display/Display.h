#pragma once
#include <GLFW/glfw3.h>
#include <string>

class Display
{
public:
	Display(std::string title, int width, int height);
	~Display();

	bool Init();
	void Update();
	void Shutdown();

	void GrabMouse() const;

	GLFWwindow* GetHandle() const;
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
	GLFWwindow* _handle;
	std::string _title;

	bool _isInitialized;
	bool _isDecorated;
	bool _isResizable;
	bool _isVisible;
	bool _isFullscreen;
	bool _maximize;

	static void CallbackWindowResize(GLFWwindow* handle, int newWidth, int newHeight);
};

