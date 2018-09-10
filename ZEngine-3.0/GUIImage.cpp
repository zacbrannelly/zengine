#include "GUIImage.h"
#include "imgui-includes.h"

using namespace bgfx;

GUIImage::GUIImage(TextureHandle texture, float width, float height) : _texture(texture), _width(width), _height(height)
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

void GUIImage::RenderElement()
{
	ImGui::Image(_texture, ImVec2(_width, _height));
}

GUIImage::~GUIImage()
{
}
