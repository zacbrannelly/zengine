#include "GUIImage.h"
#include "imgui-includes.h"

using namespace bgfx;

GUIImage::GUIImage(TextureHandle texture, float width, float height) : _texture(texture), _width(width), _height(height), _flippedVertically(false)
{
	
}

void GUIImage::SetSize(float width, float height)
{
	_width = width;
	_height = height;
}

float GUIImage::GetWidth() const
{
	return _width;
}

float GUIImage::GetHeight() const
{
	return _height;
}

void GUIImage::SetTexture(bgfx::TextureHandle texture)
{
	_texture = texture;
}

TextureHandle GUIImage::GetTexture() const
{
	return _texture;
}

void GUIImage::FlipVertically()
{
	_flippedVertically = !_flippedVertically;
}

bool GUIImage::IsFlippedVertically() const
{
	return _flippedVertically;
}

void GUIImage::RenderElement()
{
	ImGui::Image(_texture, ImVec2(_width, _height), ImVec2(0, _flippedVertically ? 1 : 0), ImVec2(1, _flippedVertically ? 0 : 1));
}

GUIImage::~GUIImage()
{
}
