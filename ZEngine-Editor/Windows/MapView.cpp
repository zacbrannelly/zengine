#include "MapView.h"
#include <ZEngine-Core/Component/FreeCamera.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Input/InputManager.h>
#include <glm/gtc/type_ptr.hpp>

#include "MapViewToolbar.h"
#include "../MapView/MapViewPicker.h"
#include "../Editor.h"
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>
#include "../Gizmos/GizmoSystem.h"

#include <iostream>

using namespace ZEngine;

MapView::MapView(Editor* editor) : GUIWindow("Map View", 1024, 850, false), _cameraInFlight(false)
{
	_editor = editor;
	_gizmoSystem = GizmoSystem::GetInstance();

	// We must create an entity (so we can transform the camera) and translate it back 10 units
	_viewEntity = Factory::CreateInstance<Entity>("View Object", ObjectType::ENTITY);
	_viewEntity->GetTransform()->SetPosition(glm::vec3(0, 0, -10));

	// Create camera on view 1, this will be used to view/render the scene
	_viewCamera = new FreeCamera();
	_viewEntity->AddComponent(_viewCamera, true);

	// Set default values for the camera
	_viewCamera->SetProjectionMode(Camera::ProjectionMode::PERSPECTIVE);
	_viewCamera->SetFieldOfView(60.0f);
	_viewCamera->SetViewport(0, 0, 1920, 1080);
	_viewCamera->SetClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	_viewCamera->SetViewId(1);
	_viewCamera->SetRenderToTexture(true);

	// Make image linked to the view camera
	_viewImage = new GUIImage(_viewCamera->GetRenderTexture(), _viewCamera->GetViewportWidth(), _viewCamera->GetViewportHeight());
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	_viewImage->FlipVertically();
#endif
    
	_mapViewToolbar = new MapViewToolbar(_editor, this);
	_mapViewPicker = new MapViewPicker(_editor, this);

	// Add elements to the window
	Add(_mapViewToolbar);
	Add(_viewImage);
}

void MapView::ProcessInput()
{
	// Activate the flying camera on right click in the map view window
	auto inputManager = InputManager::GetInstance();
	if (ImGui::IsWindowHovered() && inputManager->GetButtonDown(BUTTON_MOUSE_RIGHT) && !_cameraInFlight)
	{
		_cameraInFlight = true;
		
		if (!inputManager->IsMouseGrabbed())
			inputManager->SetMouseGrabbed(true);
	}

	// Update the camera if it is in flight
	if (_viewCamera != nullptr && _cameraInFlight)
	{
		_viewCamera->Update();
	}

	// Deactivate the flying camera on right click up
	if (inputManager->GetButtonUp(BUTTON_MOUSE_RIGHT) && _cameraInFlight)
	{
		_cameraInFlight = false;
		inputManager->SetMouseGrabbed(false);
	}

	_mapViewPicker->UpdateAABBs();
	if (!ImGuizmo::IsUsingAny() && ImGui::IsWindowHovered() && inputManager->GetButtonPressed(BUTTON_MOUSE_LEFT))
	{
		auto mouseScreenPos = inputManager->GetMousePos();
		auto viewImageScreenPos = _viewImage->GetScreenPosition();
		int mouseX = (int)(mouseScreenPos.x - viewImageScreenPos.x + 0.5f);
		int mouseY = (int)(mouseScreenPos.y - viewImageScreenPos.y + 0.5f);

		mouseX = mouseX / _viewImage->GetWidth() * _viewCamera->GetViewportWidth();
		mouseY = mouseY / _viewImage->GetHeight() * _viewCamera->GetViewportHeight();

		auto entityPicked = _mapViewPicker->Pick(mouseX, mouseY, _viewCamera);
		_editor->SetSelectedEntity(entityPicked);
	}
}

void MapView::RenderInWindow()
{
	auto viewportWidth = GetContentWidth();
	auto viewportHeight = GetContentHeight() - 28;
	auto viewportChanged = (
		_viewCamera->GetViewportWidth() != viewportWidth ||
		_viewCamera->GetViewportHeight() != viewportHeight
	);
	if (viewportWidth > 0 && viewportHeight > 0 && viewportChanged)
	{
		_viewCamera->SetViewport(0, 0, viewportWidth, viewportHeight);
	}
	_viewImage->SetSize(viewportWidth, viewportHeight);

	// Render where the view image is positioned in screen space.
	auto viewImagePos = _viewImage->GetScreenPosition();
	ImGuizmo::SetRect(viewImagePos.x, viewImagePos.y, _viewImage->GetWidth(), _viewImage->GetHeight());
	
	// Make sure the guizmo is rendered using the current window's draw list.
	ImGuizmo::SetDrawlist();

	// Render Translation/Rotate/Scale Gizmos for the selected object.
	auto selectedEntity = _editor->GetSelectedEntity();
	if (selectedEntity != nullptr)
	{
		const float* projMatrix = glm::value_ptr(_viewCamera->GetProjectionMatrix());
		const float* viewMatrix = glm::value_ptr(_viewCamera->GetViewMatrix());

		float modelMatrix[16];
		memcpy(modelMatrix, glm::value_ptr(selectedEntity->GetTransform()->GetWorldTransformMatrix()), sizeof(float) * 16);

		if (ImGuizmo::Manipulate(viewMatrix, projMatrix, _mapViewToolbar->GetObjectGizmoOp(), _mapViewToolbar->GetObjectGizmoMode(), modelMatrix))
		{
			float translation[3];
			float rotation[3];
			float scale[3];
			ImGuizmo::DecomposeMatrixToComponents(modelMatrix, translation, rotation, scale);
			
			selectedEntity->GetTransform()->SetPosition({ translation[0], translation[1], translation[2] });
			selectedEntity->GetTransform()->SetRotation({ rotation[0], rotation[1], rotation[2] });
			selectedEntity->GetTransform()->SetScale({ scale[0], scale[1], scale[2] });
		}
	}
}

void MapView::RenderElement()
{
	// Ensure the render texture is current, as it changes when viewport is changed
	_viewImage->SetTexture(_viewCamera->GetRenderTexture());

	// Set camera settings
	_viewCamera->Render(-1);

	// Render the world without the internal cameras
	if (_editor->GetSelectedMap() != nullptr)
	{
		// Render the world to the render texture
		auto map = _editor->GetSelectedMap();
		auto viewId = _viewCamera->GetViewId();
		map->RenderWorld(viewId);

		// Render the gizmos (UI elements) to the render texture
		_gizmoSystem->RenderGizmos(viewId);
	}

	// Render the actual texture to the screen (more like submit the draw call to bgfx)
	GUIWindow::RenderElement();
}

Camera* MapView::GetCamera() const
{
	return _viewCamera;
}

MapView::~MapView()
{
	delete _mapViewPicker;
}
