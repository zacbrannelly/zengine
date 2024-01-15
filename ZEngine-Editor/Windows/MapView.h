#pragma once

#include "../UI/GUIWindow.h"
#include "../UI/GUIImage.h"
#include <ZEngine-Core/Rendering/Graphics.h>

class Editor;
class GizmoSystem;
class Camera;
class Entity;
class TransformInspector;
class CameraInspector;
class Map;
class MapViewToolbar;
class MapViewPicker;

class MapView : public GUIWindow
{
public:
	MapView(Editor* map);
	~MapView();

	void ProcessInput() override;
	void RenderInWindow() override;
	void RenderElement() override;

private:
	GUIImage* _viewImage;
	Entity* _viewEntity;
	Camera* _viewCamera;
	Editor* _editor;
	GizmoSystem* _gizmoSystem;
	MapViewToolbar* _mapViewToolbar;
	MapViewPicker* _mapViewPicker;

	TransformInspector* _transformInspector;
	CameraInspector* _cameraInspector;

	float _aspectRatioW, _aspectRatioH;
	bool _cameraInFlight;
};

