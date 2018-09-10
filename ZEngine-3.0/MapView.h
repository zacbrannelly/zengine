#pragma once

#include "GUIWindow.h"
#include "GUIImage.h"
#include <ZEngine-Core/Rendering/Graphics.h>

class MapView : public GUIWindow
{
public:
	MapView();
	~MapView();

	void ProcessInput() override;
	void RenderElement() override;
private:
	bgfx::FrameBufferHandle _fbo;
	GUIImage* _viewImage;
};

