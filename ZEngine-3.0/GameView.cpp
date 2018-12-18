#include "GameView.h"
#include "Editor.h"

#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Map/Map.h>

#include "imgui-includes.h"

GameView::GameView(Editor* editor) : GUIWindow("Game View", 1024, 600, false)
{
	_editor = editor;

	SetupCamera();
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar);
}

void GameView::SetupCamera()
{
	// We can't setup without a map or camera
	if (_editor->GetSelectedMap() != nullptr && _editor->GetSelectedMap()->GetCameras().size() > 0)
	{
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
			_viewImage->FlipVertically();
			Add(_viewImage);
		}
		else
		{
			_viewImage->SetTexture(_mainCamera->GetRenderTexture());
			_viewImage->SetSize(_mainCamera->GetViewportWidth(), _mainCamera->GetViewportHeight());
		}
	}
}

void GameView::ProcessInput()
{
}

void GameView::RenderInWindow()
{

}

void GameView::RenderElement()
{
	// If no map, do nothing, if there is one, check if the camera is setup.
	if (_mainCamera == nullptr)
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
	_viewImage->SetSize(_mainCamera->GetViewportWidth(), _mainCamera->GetViewportHeight());

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
