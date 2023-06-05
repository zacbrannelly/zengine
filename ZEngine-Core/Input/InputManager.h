#pragma once

#include "../Misc/Singleton.h"
#include "InputTypes.h"
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <algorithm>
#include <GLFW/glfw3.h>

class Display;

class InputManager : public Singleton<InputManager>
{
public:
	void Init(Display* display);
	void Reset();
	void Shutdown();

	bool LoadConfiguration(std::string path);
	void RegisterAxis(std::string name, ButtonCode code);

	bool GetButtonDown(ButtonCode code);
	bool GetButtonPressed(ButtonCode code);
	bool GetButtonUp(ButtonCode code);
	bool ButtonHasModifier(ButtonCode button, ButtonCode mod);
	float GetAxis(std::string name);
	float GetAxisRaw(std::string name);

	glm::vec2 GetMousePos() const;

	void RegisterKeyCallback(GLFWkeyfun cb);
	void RegisterMousePosCallback(GLFWcursorposfun cb);
	void RegisterMouseButtonCallback(GLFWmousebuttonfun cb);

	void RemoveKeyCallback(GLFWkeyfun cb);
	void RemoveMousePosCallback(GLFWcursorposfun cb);
	void RemoveMouseButtonCallback(GLFWmousebuttonfun cb);
private:
	void UpdateButton(ButtonCode code, int action, int mods);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePositionCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	glm::vec2 _mousePos;
	std::map<ButtonCode, bool> _buttonDown;
	std::map<ButtonCode, bool> _prevButtonDown;
	std::map<ButtonCode, bool> _buttonUp;
	std::map<ButtonCode, int> _buttonModifiers;
	std::map<ButtonCode, float> _buttonAxis;
	std::map<std::string, ButtonCode> _axisMapping;

	std::vector<GLFWkeyfun> _keyCallbacks;
	std::vector<GLFWcursorposfun> _mousePosCallbacks;
	std::vector<GLFWmousebuttonfun> _mouseButtonCallbacks;
};

