#pragma once

#include "../Misc/Singleton.h"
#include "InputTypes.h"
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <algorithm>

#include <TargetConditionals.h>

#if !TARGET_OS_IPHONE
#include <GLFW/glfw3.h>
#endif

namespace ZEngine
{
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

		void SetMouseTransform(const glm::mat4& transform);
		const glm::mat4& GetMouseTransform() const;

		const glm::vec2& GetMousePos() const;
		const glm::vec2& GetMouseDelta() const;

		bool GetTouchDown(int index);
		bool GetTouchPressed(int index);
		bool GetTouchUp(int index);
		const glm::vec2& GetTouchPos(int index) const;
		static void TouchCallback(int index, int action, float x, float y);

	#if !TARGET_OS_IPHONE
		void RegisterKeyCallback(GLFWkeyfun cb);
		void RegisterMousePosCallback(GLFWcursorposfun cb);
		void RegisterMouseButtonCallback(GLFWmousebuttonfun cb);

		void RemoveKeyCallback(GLFWkeyfun cb);
		void RemoveMousePosCallback(GLFWcursorposfun cb);
		void RemoveMouseButtonCallback(GLFWmousebuttonfun cb);
	#endif
			
		void ClearMouseDelta();
		void SetMouseGrabbed(bool grabbed);
		bool IsMouseGrabbed() const;

	private:
		void UpdateButton(ButtonCode code, int action, int mods);

	#if !TARGET_OS_IPHONE
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MousePositionCallback(GLFWwindow* window, double x, double y);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	#endif

		glm::mat4 _mouseTransform;
		glm::vec2 _mousePos;
		glm::vec2 _rawMousePos;
		std::map<ButtonCode, bool> _buttonDown;
		std::map<ButtonCode, bool> _prevButtonDown;
		std::map<ButtonCode, bool> _buttonUp;
		std::map<ButtonCode, int> _buttonModifiers;
		std::map<ButtonCode, float> _buttonAxis;
		std::map<std::string, ButtonCode> _axisMapping;

	#if !TARGET_OS_IPHONE
		std::vector<GLFWkeyfun> _keyCallbacks;
		std::vector<GLFWcursorposfun> _mousePosCallbacks;
		std::vector<GLFWmousebuttonfun> _mouseButtonCallbacks;
	#endif

		std::map<int, bool> _touchDown;
		std::map<int, bool> _prevTouchDown;
		std::map<int, bool> _touchUp;
		std::map<int, glm::vec2> _touchPos;
		std::map<int, glm::vec2> _touchDelta;
			
		bool _isMouseGrabbed;
		glm::vec2 _mouseDelta;
		glm::vec2 _emptyVector;
		Display* _display;
	};
}
