#include "InputManager.h"
#include "../Display/Display.h"
#include "../Scripting/ScriptSystem.h"
#include <iostream>


void InputManager::Init(Display* display)
{
	// Setup callbacks
	glfwSetKeyCallback(display->GetHandle(), &InputManager::KeyCallback);
	glfwSetCursorPosCallback(display->GetHandle(), &InputManager::MousePositionCallback);
	glfwSetMouseButtonCallback(display->GetHandle(), &InputManager::MouseButtonCallback);
}

void InputManager::Reset()
{
	for (auto& pair : _buttonUp)
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

bool InputManager::GetButtonUp(ButtonCode code)
{
	if (_buttonUp.find(code) == _buttonUp.end())
		_buttonUp[code] = false;

	return _buttonUp[code];
}

glm::vec2 InputManager::GetMousePos() const
{
	return _mousePos;
}

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

void InputManager::UpdateButton(ButtonCode code, int action)
{
	if (action == GLFW_PRESS)
	{
		_buttonDown[(ButtonCode)code] = true;

		// TODO: Smooth the values
		_buttonAxis[(ButtonCode)code] = 1.0f;
	}
	else if (action == GLFW_RELEASE)
	{
		_buttonDown[(ButtonCode)code] = false;
		_buttonUp[(ButtonCode)code] = true;

		// TODO: Smooth the values
		_buttonAxis[(ButtonCode)code] = 0.0f;
	}
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Update the internal state
	auto instance = InputManager::GetInstance();
	instance->UpdateButton((ButtonCode)key, action);

	// Call registered callbacks
	for (auto callback : instance->_keyCallbacks)
	{
		(*callback)(window, key, scancode, action, mods);
	}
}

void InputManager::MousePositionCallback(GLFWwindow* window, double x, double y)
{
	auto instance = InputManager::GetInstance();

	// Set the internal mouse position
	instance->_mousePos.x = x;
	instance->_mousePos.y = y;

	// Call registered callbacks
	for (auto callback : instance->_mousePosCallbacks)
	{
		(*callback)(window, x, y);
	}
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// Update the internal state
	auto instance = InputManager::GetInstance();
	instance->UpdateButton((ButtonCode)button, action);

	// Call registered callbacks
	for (auto callback : instance->_mouseButtonCallbacks)
	{
		(*callback)(window, button, action, mods);
	}
}

void InputManager_Getter(v8::Local<v8::String> nameObj, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	auto input = InputManager::GetInstance();

	// Convert property name to a string
	v8::String::Utf8Value nameUtf(info.GetIsolate(), nameObj);
	std::string name(*nameUtf);

	// Check the property name used and return the corresponding value
	if (name == "mouseX")
	{
		// Return mouse x
		info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), input->GetMousePos().x));
	}
	else if (name == "mouseY")
	{
		// Return mouse y
		info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), input->GetMousePos().y));
	}
}

void InputManager_Setter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	auto input = InputManager::GetInstance();
}

void InputManager_GetButtonDown(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() != 1 || !info[0]->IsInt32())
	{
		info.GetReturnValue().Set(false);
		return;
	}

	auto input = InputManager::GetInstance();
	auto sys = ScriptSystem::GetInstance();

	// Extract the code form the args
	auto code = (ButtonCode)info[0]->ToInt32(info.GetIsolate())->Int32Value(sys->GetContext()->GetLocal()).ToChecked();

	info.GetReturnValue().Set(input->GetButtonDown(code));
}

void InputManager::SetupScriptBindings(v8::Isolate* isolate, v8::Local<v8::Object>& global)
{
	using namespace v8;
	auto sys = ScriptSystem::GetInstance();

	auto temp = ObjectTemplate::New(isolate);
	temp->SetAccessor(sys->GetString("mouseX"), InputManager_Getter, InputManager_Setter);
	temp->SetAccessor(sys->GetString("mouseY"), InputManager_Getter, InputManager_Setter);

	auto inputObj = temp->NewInstance();

	inputObj->Set(sys->GetString("GetButtonDown"), Function::New(isolate, InputManager_GetButtonDown));

	global->Set(sys->GetString("Input"), inputObj);
}

void InputManager::Shutdown()
{

}
