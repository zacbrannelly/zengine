#pragma once

#include "GUIWindow.h"
#include "GUIImage.h"
#include <ZEngine-Core/Rendering/Graphics.h>

class Map;
class Camera;
class Entity;

class MapView : public GUIWindow
{
public:
	MapView(Map* map);
	~MapView();

	void ProcessInput() override;
	void RenderInWindow() override;
	void RenderElement() override;
private:
	GUIImage* _viewImage;
	Map* _map;

	Entity* _viewEntity;
	Camera* _viewCamera;
};

