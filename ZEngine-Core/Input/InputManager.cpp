#include "InputManager.h"
#include "../Display/Display.h"
#include <iostream>

using namespace ZEngine;

void InputManager::Init(Display* display)
{
	_display = display;
	_isMouseGrabbed = false;
	_mouseTransform = glm::mat4(1.0f);
	_emptyVector = glm::vec2(0.0f, 0.0f);

	// Setup callbacks
#if !TARGET_OS_IPHONE
	glfwSetKeyCallback(display->GetHandle(), &InputManager::KeyCallback);
	glfwSetCursorPosCallback(display->GetHandle(), &InputManager::MousePositionCallback);
	glfwSetMouseButtonCallback(display->GetHandle(), &InputManager::MouseButtonCallback);
#endif
}

void InputManager::SetMouseGrabbed(bool grabbed)
{
	_isMouseGrabbed = grabbed;

#if !TARGET_OS_IPHONE
	// Set the mouse position to the center of the screen (to avoid weird jumps)
	glfwSetCursorPos(_display->GetHandle(), _display->GetWidth() / 2, _display->GetHeight() / 2);
	_mousePos = glm::vec2(_display->GetWidth() / 2, _display->GetHeight() / 2);

	// Set the cursor mode
	glfwSetInputMode(_display->GetHandle(), GLFW_CURSOR, _isMouseGrabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
#endif
}

bool InputManager::IsMouseGrabbed() const
{
	return _isMouseGrabbed;
}

const glm::vec2& InputManager::GetMouseDelta() const
{
	return _mouseDelta;
}

void InputManager::ClearMouseDelta()
{
	_mouseDelta = glm::vec2(0.0f, 0.0f);
}

void InputManager::Reset()
{
	for (auto& pair : _prevButtonDown)
	{
		pair.second = _buttonDown[pair.first];
	}

	for (auto& pair : _buttonUp)
	{
		pair.second = false;
	}
    
    for (auto& pair : _prevTouchDown)
    {
        pair.second = _touchDown[pair.first];
    }
    
    for (auto& pair : _touchUp)
    {
        pair.second = false;
    }
}

bool InputManager::LoadConfiguration(std::string path)
{
	return false;
}

void InputManager::RegisterAxis(std::string name, ButtonCode code)
{
	_axisMapping[name] = code;
}

float InputManager::GetAxis(std::string name)
{
	auto it = _axisMapping.find(name);

	if (it != _axisMapping.end())
	{
		ButtonCode code = (*it).second;

		auto axisIt = _buttonAxis.find(code);

		if (axisIt != _buttonAxis.end())
		{
			return (*axisIt).second;
		}
	}
	else
	{
		std::cout << "INPUT_MANAGER: Failed to find the axis '" << name << "' in the mappings!" << std::endl;
	}

	return 0.0f;
}

float InputManager::GetAxisRaw(std::string name)
{
	// TODO: Smooth the axis values 
	return GetAxis(name);
}

bool InputManager::GetButtonDown(ButtonCode code)
{
	if (_buttonDown.find(code) == _buttonDown.end())
		_buttonDown[code] = false;

	return _buttonDown[code];
}

bool InputManager::GetButtonPressed(ButtonCode code)
{
	if (_buttonDown.find(code) == _buttonDown.end())
	{
		return false;
	}

	if (_prevButtonDown.find(code) == _prevButtonDown.end())
	{
		return false;
	}

	return _buttonDown[code] && !_prevButtonDown[code];
}

bool InputManager::GetButtonUp(ButtonCode code)
{
	if (_buttonUp.find(code) == _buttonUp.end())
		_buttonUp[code] = false;

	return _buttonUp[code];
}

bool InputManager::ButtonHasModifier(ButtonCode button, ButtonCode mod)
{
	if (_buttonModifiers.find(button) != _buttonModifiers.end())
	{
		return _buttonModifiers[button] & mod;
	}

	return false;
}

void InputManager::SetMouseTransform(const glm::mat4& transform)
{
	_mouseTransform = transform;

	// Apply the transform to the current mouse position
	glm::vec3 mousePos = _mouseTransform * glm::vec4(_rawMousePos, 1.0f, 1.0f);
	_mousePos = glm::vec2(mousePos.x, mousePos.y);
}

const glm::mat4& InputManager::GetMouseTransform() const
{
	return _mouseTransform;
}

const glm::vec2& InputManager::GetMousePos() const
{
	return _mousePos;
}

#if !TARGET_OS_IPHONE
void InputManager::RegisterKeyCallback(GLFWkeyfun cb)
{
	_keyCallbacks.push_back(cb);
}

void InputManager::RegisterMousePosCallback(GLFWcursorposfun cb)
{
	_mousePosCallbacks.push_back(cb);
}

void InputManager::RegisterMouseButtonCallback(GLFWmousebuttonfun cb)
{
	_mouseButtonCallbacks.push_back(cb);
}

void InputManager::RemoveKeyCallback(GLFWkeyfun cb)
{
	auto it = find(_keyCallbacks.begin(), _keyCallbacks.end(), cb);
	
	if (it != _keyCallbacks.end())
		_keyCallbacks.erase(it);
}

void InputManager::RemoveMousePosCallback(GLFWcursorposfun cb)
{
	auto it = find(_mousePosCallbacks.begin(), _mousePosCallbacks.end(), cb);

	if (it != _mousePosCallbacks.end())
		_mousePosCallbacks.erase(it);
}

void InputManager::RemoveMouseButtonCallback(GLFWmousebuttonfun cb)
{
	auto it = find(_mouseButtonCallbacks.begin(), _mouseButtonCallbacks.end(), cb);

	if (it != _mouseButtonCallbacks.end())
		_mouseButtonCallbacks.erase(it);
}
#endif

void InputManager::UpdateButton(ButtonCode code, int action, int mods)
{
	if (action == BUTTON_ACTION_PRESS)
	{
		if (_buttonDown.find(code) != _buttonDown.end())
		{
			_prevButtonDown[code] = _buttonDown[code];
		}

		_buttonDown[code] = true;

		// TODO: Smooth the values
		_buttonAxis[code] = 1.0f;
	}
	else if (action == BUTTON_ACTION_RELEASE)
	{
		if (_buttonDown.find(code) != _buttonDown.end())
		{
			_prevButtonDown[code] = _buttonDown[code];
		}

		_buttonDown[code] = false;
		_buttonUp[code] = true;

		// TODO: Smooth the values
		_buttonAxis[code] = 0.0f;
	}

	_buttonModifiers[code] = mods;
}

#if !TARGET_OS_IPHONE
void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Update the internal state
	auto instance = InputManager::GetInstance();
	instance->UpdateButton((ButtonCode)key, action, mods);

	// Call registered callbacks
	for (auto callback : instance->_keyCallbacks)
	{
		(*callback)(window, key, scancode, action, mods);
	}
}

void InputManager::MousePositionCallback(GLFWwindow* window, double x, double y)
{
	auto instance = InputManager::GetInstance();

	instance->_rawMousePos.x = (float)x;
	instance->_rawMousePos.y = (float)y;

	glm::vec3 mousePos = instance->_mouseTransform * glm::vec4(x, y, 1, 1);
	glm::vec2 transformedMousePos = glm::vec2(mousePos.x, mousePos.y);

	// Update the mouse delta
	instance->_mouseDelta = transformedMousePos - instance->_mousePos;

	// Set the internal mouse position
	instance->_mousePos = transformedMousePos;

	// Call registered callbacks
	for (auto callback : instance->_mousePosCallbacks)
	{
		(*callback)(window, transformedMousePos.x, transformedMousePos.y);
	}
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// Update the internal state
	auto instance = InputManager::GetInstance();
	instance->UpdateButton((ButtonCode)button, action, mods);

	// Call registered callbacks
	for (auto callback : instance->_mouseButtonCallbacks)
	{
		(*callback)(window, button, action, mods);
	}
}
#endif

bool InputManager::GetTouchDown(int index)
{
	if (_touchDown.find(index) == _touchDown.end())
		_touchDown[index] = false;

	return _touchDown[index];
}

bool InputManager::GetTouchPressed(int index)
{
	if (_touchDown.find(index) == _touchDown.end())
	{
		return false;
	}

	if (_prevTouchDown.find(index) == _prevTouchDown.end())
	{
		return false;
	}

	return _touchDown[index] && !_prevTouchDown[index];
}

bool InputManager::GetTouchUp(int index)
{
	if (_touchUp.find(index) == _touchUp.end())
		_touchUp[index] = false;

	return _touchUp[index];
}

const glm::vec2& InputManager::GetTouchPos(int index) const
{
	if (_touchPos.find(index) == _touchPos.end())
		return _emptyVector;

	return _touchPos.at(index);
}

void InputManager::TouchCallback(int index, int action, float x, float y)
{
	auto instance = InputManager::GetInstance();

	if (action == BUTTON_ACTION_PRESS)
	{
		if (instance->_touchDown.find(index) != instance->_touchDown.end())
		{
			instance->_prevTouchDown[index] = instance->_touchDown[index];
		}

		instance->_touchDown[index] = true;
	}
	else if (action == BUTTON_ACTION_RELEASE)
	{
		if (instance->_touchDown.find(index) != instance->_touchDown.end())
		{
			instance->_prevTouchDown[index] = instance->_touchDown[index];
		}

		instance->_touchDown[index] = false;
		instance->_touchUp[index] = true;
	}

	instance->_touchPos[index] = glm::vec2(x, y);
}

void InputManager::Shutdown()
{
}
