#include "GameView.h"
#include "Editor.h"

#include <ZEngine-Core/Component/Camera.h>
#include <ZEngine-Core/Map/Map.h>

#include "imgui-includes.h"

GameView::GameView(Editor* editor) : GUIWindow("Game View", 1024, 600, false)
{
	_editor = editor;

	_mainCamera = _editor->GetSelectedMap()->GetCameras()[0];
	_mainCamera->SetRenderToTexture(true);
	_mainCamera->SetViewId(2);

	SetSize(_mainCamera->GetViewportWidth(), _mainCamera->GetViewportHeight());

	// Make image linked to the view camera then add it as a GUI element
	_viewImage = new GUIImage(_mainCamera->GetRenderTexture(), _mainCamera->GetViewportWidth(), _mainCamera->GetViewportHeight());
	Add(_viewImage);
	
	SetFlags(ImGuiWindowFlags_AlwaysAutoResize);
}

void GameView::ProcessInput()
{
}

void GameView::RenderInWindow()
{

}

void GameView::RenderElement()
{
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
