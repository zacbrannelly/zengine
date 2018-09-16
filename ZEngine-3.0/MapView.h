#pragma once

#include "GUIWindow.h"
#include "GUIImage.h"
#include <ZEngine-Core/Rendering/Graphics.h>

class Editor;
class Camera;
class Entity;
class TransformInspector;
class CameraInspector;

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

	TransformInspector* _transformInspector;
	CameraInspector* _cameraInspector;

	bool _isPlaying;
};

