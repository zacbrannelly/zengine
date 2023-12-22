#pragma once

#include "../UI/GUIWindow.h"
#include "../UI/GUIImage.h"
#include <ZEngine-Core/Rendering/Graphics.h>

class Editor;
class Camera;
class Entity;
class TransformInspector;
class CameraInspector;
class Map;
class MapViewToolbar;

class MapView : public GUIWindow
{
public:
	MapView(Editor* map);
	~MapView();

	void Update() override;
	void ProcessInput() override;
	void RenderInWindow() override;
	void RenderElement() override;

private:
	GUIImage* _viewImage;
	Entity* _viewEntity;
	Camera* _viewCamera;
	Editor* _editor;
    MapViewToolbar* _mapViewToolbar;

	TransformInspector* _transformInspector;
	CameraInspector* _cameraInspector;

	float _aspectRatioW, _aspectRatioH;
};

