#include "MapView.h"
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Transform.h>

#include "imgui-includes.h"


MapView::MapView(Map* map) : GUIWindow("Map View", 1024, 640, false)
{
	_map = map;

	// We must create an entity (so we can transform the camera) and translate it back 10 units
	_viewEntity = Factory::CreateInstance<Entity>("View Object", ObjectType::ENTITY);
	_viewEntity->GetTransform()->SetPosition(glm::vec3(0, 0, -10));

	// Create camera on view 1, this will be used to view/render the scene
	_viewCamera = Factory::CreateInstance<Camera>("Camera", ObjectType::CAMERA);	
	_viewEntity->AddComponent(_viewCamera);

	_viewCamera->SetProjectionMode(Camera::ProjectionMode::ORTHOGRAPHIC);
	_viewCamera->SetFieldOfView(90.0f);
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

	auto transform = _viewEntity->GetTransform();
	float newPos[3] = { transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z };
	if (ImGui::SliderFloat3("Position", newPos, -10, 10))
	{
		transform->SetPosition({ newPos[0], newPos[1], newPos[2] });
	}

	float deg2rad = 3.1415f / 180.0f;
	float newRot[3] = { transform->GetRotaion().x / deg2rad, transform->GetRotaion().y / deg2rad, transform->GetRotaion().z / deg2rad };
	if (ImGui::SliderFloat3("Rotation", newRot, 0, 360))
	{
		transform->SetRotation({ newRot[0] * deg2rad, newRot[1] * deg2rad, newRot[2] * deg2rad });
	}

	if (ImGui::BeginCombo("Projection", _viewCamera->GetProjectionMode() == Camera::PERSPECTIVE ? "Perspective" : "Orthograpic"))
	{
		if (ImGui::Selectable("Orthographic"))
		{
			_viewCamera->SetProjectionMode(Camera::ORTHOGRAPHIC);
		}

		if (ImGui::Selectable("Perspective"))
		{
			_viewCamera->SetProjectionMode(Camera::PERSPECTIVE);
		}

		ImGui::EndCombo();
	}

	float fov = _viewCamera->GetFieldOfView();
	if (ImGui::SliderAngle("Field of View", &fov))
	{
		_viewCamera->SetFieldOfView(glm::degrees(fov));
	}

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
