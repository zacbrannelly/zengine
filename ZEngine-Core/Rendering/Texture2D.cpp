#include "Texture2D.h"

#include "stb_image.h"

Texture2D::Texture2D(uint16_t width, uint16_t height, bool hasMips, uint16_t numLayers, bgfx::TextureFormat::Enum format, uint64_t flags, void* data, uint32_t size)
{
	_handle = bgfx::createTexture2D(width, height, hasMips, numLayers, format, flags, data ? bgfx::makeRef(data, size) : NULL);
	_width = width;
	_height = height;
}

void Texture2D::Update(
	uint16_t x,
	uint16_t y,
	uint16_t width,
	uint16_t height,
	const void* data,
	const uint32_t size,
	uint16_t pitch,
	uint16_t layer,
	uint8_t mip
)
{
	bgfx::updateTexture2D(_handle, layer, mip, x, y, width, height, bgfx::copy(data, size));
}

bool Texture2D::IsValid() const
{
	return _handle.idx != bgfx::kInvalidHandle;
}

bgfx::TextureHandle Texture2D::GetHandle() const
{
	return _handle;
}

int Texture2D::GetWidth() const
{
	return _width;
}

int Texture2D::GetHeight() const
{
	return _height;
}

Texture2D::~Texture2D()
{
}
