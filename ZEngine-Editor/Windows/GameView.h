#pragma once

#include <ZEngine-Core/ImmediateUI/GUIWindow.h>
#include <ZEngine-Core/ImmediateUI/GUIImage.h>
#include <ZEngine-Core/Rendering/Graphics.h>

namespace ZEngine
{
	class Editor;
	class Camera;
	class Entity;
	class Map;
	class TransformInspector;
	class CameraInspector;

	enum GameViewSizeMode
	{
		FIT,
		SCALED,
	};

	class GameView : public GUIWindow
	{
	public:
		GameView(Editor* map);
		~GameView();

		void SetupCamera();

		void ProcessInput() override;
		void RenderInWindowBeforeElements() override;
		void RenderInWindow() override;
		void RenderElement() override;
	private:
		GUIImage* _viewImage;
		Editor* _editor;
		Camera* _mainCamera;
		Map* _currentMap;
		GameViewSizeMode _sizeMode;
		float _scale;
	};
}
