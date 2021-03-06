#include "MapView.h"
#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Audio/AudioSystem.h>
#include <ZEngine-Core/Map/MapManager.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/Objects/ScriptAsset.h>
#include <ZEngine-Core/Scripting/Script.h>

#include "Editor.h"
#include "TransformInspector.h"
#include "CameraInspector.h"
#include "imgui-includes.h"


MapView::MapView(Editor* editor) : GUIWindow("Map View", 1024, 850, false)
{
	_editor = editor;
	_isPlaying = false;
	_isPaused = false;
	_previewMap = nullptr;

	// We must create an entity (so we can transform the camera) and translate it back 10 units
	_viewEntity = Factory::CreateInstance<Entity>("View Object", ObjectType::ENTITY);
	_viewEntity->GetTransform()->SetPosition(glm::vec3(0, 0, -10));

	// Create camera on view 1, this will be used to view/render the scene
	_viewCamera = Factory::CreateInstance<Camera>("Camera", ObjectType::CAMERA);	
	_viewEntity->AddComponent(_viewCamera);

	// Set default values for the camera
	_viewCamera->SetProjectionMode(Camera::ProjectionMode::ORTHOGRAPHIC);
	_viewCamera->SetFieldOfView(60.0f);
	_viewCamera->SetViewport(0, 0, 1920, 1080);
	_viewCamera->SetClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	_viewCamera->SetViewId(1);
	_viewCamera->SetRenderToTexture(true);

	// Make image linked to the view camera then add it as a GUI element
	_viewImage = new GUIImage(_viewCamera->GetRenderTexture(), _viewCamera->GetViewportWidth(), _viewCamera->GetViewportHeight());
	_viewImage->FlipVertically();
	Add(_viewImage);

	_aspectRatioW = 16;
	_aspectRatioH = 9;

	_transformInspector = new TransformInspector();
	_cameraInspector = new CameraInspector();

	_transformInspector->Inspect(_viewEntity->GetTransform());
	_cameraInspector->Inspect(_viewCamera);

	// The window won't be resizable, it will fit to content
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
}

void MapView::Play()
{
	if (_isPlaying || _editor->GetSelectedMap() == nullptr)
		return;

	// Ensure sound will work
	auto audioSys = AudioSystem::GetInstance();
	audioSys->Resume(-1);
	audioSys->ResumeMusic();
	
	// If paused, resume the game
	if (_isPaused)
	{
		_isPaused = false;
		_isPlaying = true;
		return;
	}

	_isPlaying = true;

	// Copy the original map (so we don't break the original during play)
	_originalMap = _editor->GetSelectedMap();
	_previewMap = Factory::Copy<Map>(_originalMap->GetName(), _originalMap);

	auto mapManager = MapManager::GetInstance();

	// Set the copy as the "selected" map (so both the editor and scripting engine know)
	mapManager->SetCurrentMap(_previewMap);
	_editor->SetSelectedMap(_previewMap);

}

void MapView::Pause()
{
	if (_editor->GetSelectedMap() == nullptr)
		return;

	if (_isPaused)
	{
		Play();
		return;
	}

	_isPlaying = false;
	_isPaused = true;

	// Pause all sound
	auto audioSys = AudioSystem::GetInstance();
	audioSys->Pause(-1);
	audioSys->PauseMusic();
}

void MapView::Stop()
{
	if ((!_isPlaying && !_isPaused) || _editor->GetSelectedMap() == nullptr)
		return;

	_isPlaying = false;
	_isPaused = false;

	// Delete the copy map
	delete _previewMap;
	_previewMap = nullptr;

	// Set selected entity to null so the inspector doesnt freak ouot
	_editor->SetSelectedEntity(nullptr);

	// Put the original map back as "selected"
	auto mapManager = MapManager::GetInstance();
	mapManager->SetCurrentMap(_originalMap);
	_editor->SetSelectedMap(_originalMap);

	// Stop all audio
	auto audioSys = AudioSystem::GetInstance();
	audioSys->Stop();
	audioSys->StopMusic();
}

void MapView::ProcessInput()
{
}

void MapView::RenderInWindow()
{
	_viewImage->SetSize(GetContentWidth(), GetContentWidth() * (_aspectRatioH / _aspectRatioW));

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
			Play();
		}
		
		ImGui::SameLine();

		bool startPause = false;
		if (_isPaused)
		{
			ImGui::PushStyleColor(0, ImVec4(1, 0, 0, 1));
			startPause = ImGui::Button("Pause", ImVec2(80, 0));
			ImGui::PopStyleColor();
		}
		else
		{
			startPause = ImGui::Button("Pause", ImVec2(80, 0));
		}

		if (startPause)
			Pause();

		ImGui::SameLine();
		if (ImGui::Button("Stop", ImVec2(80, 0)))
		{
			Stop();
		}
	}

	// Allow the user to change the size of the texture being rendered in the window
	if (ImGui::CollapsingHeader("View Settings"))
	{
		float aspectRatio[] = { _aspectRatioW, _aspectRatioH };
		if (ImGui::InputFloat2("Aspect Ratio", aspectRatio, "%.1f"))
		{
			_aspectRatioW = aspectRatio[0];
			_aspectRatioH = aspectRatio[1];

			_viewImage->SetSize(GetContentWidth(), GetContentWidth() * (_aspectRatioH / _aspectRatioW));
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
		if (_isPlaying)
		{
			_editor->GetSelectedMap()->Update();
		}

		_editor->GetSelectedMap()->RenderWorld(_viewCamera->GetViewId());
	}

	// Render the actual texture to the screen (more like submit the draw call to bgfx)
	GUIWindow::RenderElement();
}

MapView::~MapView()
{
}
