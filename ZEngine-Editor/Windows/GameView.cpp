#include "GameView.h"
#include "../Editor.h"

#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Display/Display.h>
#include <ZEngine-Core/Input/InputManager.h>
#include "../Controllers/MapController.h"

#include <glm/gtc/matrix_transform.hpp>
#include "../imgui-includes.h"

GameView::GameView(Editor* editor) 
	: GUIWindow("Game View", 1024, 600, false),
	  _viewImage(nullptr),
	 	_editor(editor),
		_mainCamera(nullptr),
		_currentMap(nullptr),
		_sizeMode(FIT),
		_scale(100.0f)
{
	SetupCamera();
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar);
}

void GameView::SetupCamera()
{
	// We can't setup without a map or camera
	if (_editor->GetSelectedMap() != nullptr && _editor->GetSelectedMap()->GetCameras().size() > 0)
	{
		// Keep current map so we can detect change
		_currentMap = _editor->GetSelectedMap();

		// Make main camera render to texture
		_mainCamera = _editor->GetSelectedMap()->GetCameras()[0];
		_mainCamera->SetRenderToTexture(true);
		_mainCamera->SetViewId(2);

		// Set size of the window to the size of the camera viewport
		SetSize(_mainCamera->GetViewportWidth(), _mainCamera->GetViewportHeight());

		// Setup the image to be drawn by ImGui
		if (_viewImage == nullptr)
		{
			_viewImage = new GUIImage(_mainCamera->GetRenderTexture(), _mainCamera->GetViewportWidth(), _mainCamera->GetViewportHeight());
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
			_viewImage->FlipVertically();
#endif
			Add(_viewImage);
		}
		else
		{
			_viewImage->SetTexture(_mainCamera->GetRenderTexture());
		}
	}
	else if (_mainCamera != nullptr)
	{
		// If we have a camera but no map, remove the camera
		_mainCamera = nullptr;
		_currentMap = nullptr;

		// Remove the image from the window
		Remove(_viewImage);
		_viewImage = nullptr;
	}
}

void GameView::ProcessInput()
{
}

void GameView::RenderInWindowBeforeElements()
{
	const char* modes[] = { "Fit", "Scaled" };
	ImGui::Combo("Size Mode", (int*)&_sizeMode, modes, 2);

	if (_sizeMode == SCALED)
	{
		ImGui::SameLine();
		if (ImGui::InputFloat("Scale", &_scale, 0.1, 500.0f, "%.1f"))
		{
			_scale = glm::clamp(_scale, 0.1f, 500.0f);
		}

		auto scaleFactor = _scale / 100.0f;
		_viewImage->SetSize(
			_mainCamera->GetViewportWidth() * scaleFactor,
			_mainCamera->GetViewportHeight() * scaleFactor
		);
	}

	if (_sizeMode == FIT && _mainCamera != nullptr)
	{
		float width = GetContentWidth();
		float height = GetContentHeight();

		// Maintain aspect ratio, and adjust width or height to fit
		if (width / height > _mainCamera->GetAspectRatio())
		{
			width = height * _mainCamera->GetAspectRatio();
		}
		else
		{
			height = width * ((float)_mainCamera->GetViewportHeight() / _mainCamera->GetViewportWidth());
		}
		_viewImage->SetSize(width, height);
	}

	auto isPlaying = _editor->GetMapController()->GetPlayState() == PlayState::PLAYING;
	if (isPlaying && ImGui::IsWindowHovered())
	{
		// Apply offset and scaling to the game mouse position
		glm::mat4 transform(1.0f);
		transform = glm::scale(transform, glm::vec3(_mainCamera->GetViewportWidth() / _viewImage->GetWidth(), _mainCamera->GetViewportHeight() / _viewImage->GetHeight(), 1.0f));
		transform = glm::translate(transform, glm::vec3(-_viewImage->GetScreenPosition(), 0.0f));
		InputManager::GetInstance()->SetMouseTransform(transform);
	} else {
		// Reset mouse transform
		InputManager::GetInstance()->SetMouseTransform(glm::mat4(1.0f));
	}
}

void GameView::RenderInWindow()
{
}

void GameView::RenderElement()
{
	// If no map, do nothing, if there is one, check if the camera is setup.
	if (_mainCamera == nullptr || _currentMap != _editor->GetSelectedMap())
	{
		SetupCamera();

		if (_mainCamera == nullptr)
		{
			GUIWindow::RenderElement();
			return;
		}
	}

	// Ensure the render texture is current, as it changes when viewport is changed
	_viewImage->SetTexture(_mainCamera->GetRenderTexture());

	// Set camera settings
	_mainCamera->Render(-1);

	// Render the world without the internal cameras
	if (_editor->GetSelectedMap() != nullptr)
	{
		_editor->GetSelectedMap()->RenderWorld(_mainCamera->GetViewId());
	}

	// Render the actual texture to the screen (more like submit the draw call to bgfx)
	GUIWindow::RenderElement();
}

GameView::~GameView()
{
}
