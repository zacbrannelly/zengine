#include "MapView.h"
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Transform.h>


MapView::MapView(Map* map) : GUIWindow("Map View", 1024, 640, false)
{
	_map = map;

	// We must create an entity (so we can transform the camera)
	_viewEntity = Factory::CreateInstance<Entity>("View Object", ObjectType::ENTITY);

	_viewEntity->GetTransform()->SetPosition(glm::vec3(0, 0, 10));

	// Create camera on view 1, this will be used to view/render the scene
	_viewCamera = Factory::CreateInstance<Camera>("Camera", ObjectType::CAMERA);	
	_viewEntity->AddComponent(_viewCamera);

	_viewCamera->SetViewport(0, 0, 1024, 600);
	_viewCamera->SetClearColor(0, 0, 1, 1.0f);
	_viewCamera->SetViewId(1);
	_viewCamera->SetRenderToTexture(true);

	// Make image linked to the view camera then add it as a GUI element
	_viewImage = new GUIImage(_viewCamera->GetRenderTexture(), 1024, 600);
	Add(_viewImage);
}

void MapView::ProcessInput()
{
	_viewImage->SetSize(GetWidth(), GetHeight() - 40);
}

void MapView::RenderElement()
{
	// Set camera settings
	_viewCamera->Render(-1);

	// Render the world without the internal cameras
	if (_map != nullptr)
	{
		_map->RenderWorld(_viewCamera->GetViewId());
	}

	// Render the actual texture to the screen (more like submit the draw call to bgfx)
	GUIWindow::RenderElement();
}

MapView::~MapView()
{
}
