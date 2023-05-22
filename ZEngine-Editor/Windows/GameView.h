#pragma once

#include "../UI/GUIWindow.h"
#include "../UI/GUIImage.h"
#include <ZEngine-Core/Rendering/Graphics.h>

class Editor;
class Camera;
class Entity;
class Map;
class TransformInspector;
class CameraInspector;

class GameView : public GUIWindow
{
public:
	GameView(Editor* map);
	~GameView();

	void SetupCamera();

	void ProcessInput() override;
	void RenderInWindow() override;
	void RenderElement() override;
private:
	GUIImage* _viewImage;
	Editor* _editor;
	Camera* _mainCamera;
	Map* _currentMap;
};
