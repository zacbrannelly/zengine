#include "MapView.h"
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Transform.h>

#include "Editor.h"
#include "TransformInspector.h"
#include "CameraInspector.h"
#include "imgui-includes.h"


MapView::MapView(Editor* editor) : GUIWindow("Map View", 1024, 850, false)
{
	_editor = editor;
	_isPlaying = false;

	// We must create an entity (so we can transform the camera) and translate it back 10 units
	_viewEntity = Factory::CreateInstance<Entity>("View Object", ObjectType::ENTITY);
	_viewEntity->GetTransform()->SetPosition(glm::vec3(0, 0, -10));

	// Create camera on view 1, this will be used to view/render the scene
	_viewCamera = Factory::CreateInstance<Camera>("Camera", ObjectType::CAMERA);	
	_viewEntity->AddComponent(_viewCamera);

	// Set default values for the camera
	_viewCamera->SetProjectionMode(Camera::ProjectionMode::ORTHOGRAPHIC);
	_viewCamera->SetFieldOfView(60.0f);
	_viewCamera->SetViewport(0, 0, 1024, 600);
	_viewCamera->SetClearColor(0, 0, 1, 1.0f);
	_viewCamera->SetViewId(1);
	_viewCamera->SetRenderToTexture(true);

	// Make image linked to the view camera then add it as a GUI element
	_viewImage = new GUIImage(_viewCamera->GetRenderTexture(), _viewCamera->GetViewportWidth(), _viewCamera->GetViewportHeight());
	Add(_viewImage);

	_transformInspector = new TransformInspector();
	_cameraInspector = new CameraInspector();

	_transformInspector->Inspect(_viewEntity->GetTransform());
	_cameraInspector->Inspect(_viewCamera);

	// The window won't be resizable, it will fit to content
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
}

void MapView::ProcessInput()
{
}

void MapView::RenderInWindow()
{
	// Playback buttons
	{
		bool startPlaying = false;
		if (_isPlaying)
		{
			ImGui::PushStyleColor(0, ImVec4(1, 0, 0, 1));
			startPlaying = ImGui::Button("Play", ImVec2(80, 0));
			ImGui::PopStyleColor();
		}
		else
		{
			startPlaying = ImGui::Button("Play", ImVec2(80, 0));
		}

		if (startPlaying)
		{
			_isPlaying = !_isPlaying;

			// TODO: Start playing the map when this is true
		}

		ImGui::SameLine();
		if (ImGui::Button("Pause", ImVec2(80, 0)))
		{

		}

		ImGui::SameLine();
		if (ImGui::Button("Stop", ImVec2(80, 0)))
		{
			_isPlaying = false;
		}
	}

	// Allow the user to change the size of the texture being rendered in the window
	if (ImGui::CollapsingHeader("View Settings"))
	{
		float screenSize[] = { _viewImage->GetWidth(), _viewImage->GetHeight() };
		if (ImGui::InputFloat2("Screen Size", screenSize, "%.1f"))
		{
			_viewImage->SetSize(screenSize[0], screenSize[1]);
		}
	}

	// Draw the inspectors for the camera transform and camera settings
	if (ImGui::CollapsingHeader("Camera Settings"))
	{
		_transformInspector->RenderElement();
		_cameraInspector->RenderElement();
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
		_editor->GetSelectedMap()->RenderWorld(_viewCamera->GetViewId());
	}

	// Render the actual texture to the screen (more like submit the draw call to bgfx)
	GUIWindow::RenderElement();
}

MapView::~MapView()
{
}
