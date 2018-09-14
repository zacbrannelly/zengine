#include "MapView.h"
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Transform.h>

#include "imgui-includes.h"


MapView::MapView(Map* map) : GUIWindow("Map View", 1024, 850, false)
{
	_map = map;

	// We must create an entity (so we can transform the camera) and translate it back 10 units
	_viewEntity = Factory::CreateInstance<Entity>("View Object", ObjectType::ENTITY);
	_viewEntity->GetTransform()->SetPosition(glm::vec3(0, 0, -10));

	// Create camera on view 1, this will be used to view/render the scene
	_viewCamera = Factory::CreateInstance<Camera>("Camera", ObjectType::CAMERA);	
	_viewEntity->AddComponent(_viewCamera);

	_viewCamera->SetProjectionMode(Camera::ProjectionMode::ORTHOGRAPHIC);
	_viewCamera->SetFieldOfView(60.0f);
	_viewCamera->SetViewport(0, 0, 1024, 600);
	_viewCamera->SetClearColor(0, 0, 1, 1.0f);
	_viewCamera->SetViewId(1);
	_viewCamera->SetRenderToTexture(true);

	// Make image linked to the view camera then add it as a GUI element
	_viewImage = new GUIImage(_viewCamera->GetRenderTexture(), 1024, 600);
	Add(_viewImage);

	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
}

void MapView::ProcessInput()
{
	//_viewImage->SetSize(GetWidth(), GetHeight() - 40);
}

void MapView::RenderInWindow()
{
	if (ImGui::CollapsingHeader("Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::BeginChild("Map View Camera Settings", ImVec2(0, 100), false, ImGuiWindowFlags_AlwaysAutoResize);

		auto transform = _viewEntity->GetTransform();
		float newPos[3] = { transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z };
		if (ImGui::DragFloat3("Position", newPos))
		{
			transform->SetPosition({ newPos[0], newPos[1], newPos[2] });
		}

		float newRot[3] = { transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z };
		if (ImGui::DragFloat3("Rotation", newRot))
		{
			transform->SetRotation({ newRot[0], newRot[1], newRot[2] });
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

		float size = _viewCamera->GetOrthoSize();
		if (ImGui::SliderFloat("Size", &size, 0.0f, 50.0f))
		{
			_viewCamera->SetOrthoSize(size);
		}

		ImGui::EndChild();
	}
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
