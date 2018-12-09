#pragma once

#include "GUIWindow.h"
#include "GUIImage.h"
#include <ZEngine-Core/Rendering/Graphics.h>

class Editor;
class Camera;
class Entity;
class TransformInspector;
class CameraInspector;
class Map;

class MapView : public GUIWindow
{
public:
	MapView(Editor* map);
	~MapView();

	void Play();
	void Pause();
	void Stop();

	void ProcessInput() override;
	void RenderInWindow() override;
	void RenderElement() override;

private:
	GUIImage* _viewImage;
	Entity* _viewEntity;
	Camera* _viewCamera;
	Editor* _editor;
	Map* _previewMap;
	Map* _originalMap;

	TransformInspector* _transformInspector;
	CameraInspector* _cameraInspector;

	bool _isPlaying;
	bool _isPaused;
};

