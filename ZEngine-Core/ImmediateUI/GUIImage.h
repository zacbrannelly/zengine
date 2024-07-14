#pragma once

#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include "GUIElement.h"

class GUIImage : public GUIElement
{
public:
	GUIImage(bgfx::TextureHandle texture, float width, float height);
	~GUIImage();

	void SetSize(float width, float height);
	float GetWidth() const;
	float GetHeight() const;
    
    glm::vec2 GetScreenPosition() const;

	void FlipVertically();
	bool IsFlippedVertically() const;

	void SetTexture(bgfx::TextureHandle texture);
	bgfx::TextureHandle GetTexture() const;

	void RenderElement() override;
	GUIElementType GetType() override;

private:
	bgfx::TextureHandle _texture;
	bool _flippedVertically;
	float _width;
	float _height;
    glm::vec2 _screenPos;
};

