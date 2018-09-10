#pragma once
#include <GLFW\glfw3.h>
#include <string>

class Display
{
public:
	Display(std::string title, int width, int height);
	~Display();

	bool Init();
	void Update();
	void Shutdown();

	GLFWwindow* GetHandle() const;
	void* GetWin32Handle() const;

	void SetTitle(std::string title);
	std::string GetTitle() const;

	void SetSize(int width, int height);

	void SetWidth(int width);
	int GetWidth() const;

	void SetHeight(int height);
	int GetHeight() const;

	bool CloseRequested() const;
private:
	int _width, _height;
	GLFWwindow* _handle;
	std::string _title;

	static void CallbackWindowResize(GLFWwindow* handle, int newWidth, int newHeight);
};

