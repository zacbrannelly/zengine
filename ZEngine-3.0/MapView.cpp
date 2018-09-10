#include "MapView.h"
#include <ZEngine-Core/Rendering/Graphics.h>


MapView::MapView() : GUIWindow("Map View", 1024, 600, false)
{
	auto graphics = Graphics::GetInstance();

	_fbo = graphics->CreateFrameBuffer(1024, 600);
	auto texture = graphics->GetFrameBufferTexture(_fbo);

	_viewImage = new GUIImage(texture, GetWidth(), GetHeight());
	Add(_viewImage);
}

void MapView::ProcessInput()
{
	_viewImage->SetSize(GetWidth(), GetHeight());
}

void MapView::RenderElement()
{
	auto graphics = Graphics::GetInstance();

	// Set the framebuffer to the view (we will use view 1 for scene rendering)
	graphics->SetFrameBuffer(1, _fbo);
	graphics->Clear(1, 255, 0, 0, 255);
	graphics->Viewport(1, 0, 0, 1024, 600);
	graphics->Touch(1);

	GUIWindow::RenderElement();
}

MapView::~MapView()
{
}
