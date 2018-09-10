#pragma once

#include <bgfx/bgfx.h>
#include "GUIElement.h"

class GUIImage : public GUIElement
{
public:
	GUIImage(bgfx::TextureHandle texture, float width, float height);
	~GUIImage();

	void SetSize(float width, float height);
	float GetWidth() const;
	float GetHeight() const;

	void RenderElement() override;

private:
	bgfx::TextureHandle _texture;
	float _width;
	float _height;
};

