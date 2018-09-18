#include "Texture2D.h"

#include "../../stb/stb_image.h"

Texture2D::Texture2D(uint16_t width, uint16_t height, bool hasMips, uint16_t numLayers, bgfx::TextureFormat::Enum format, uint64_t flags, void* data, uint32_t size)
{
	_handle = bgfx::createTexture2D(width, height, false, 1, format, flags, bgfx::makeRef(data, size));
}

bool Texture2D::IsValid() const
{
	return _handle.idx != bgfx::kInvalidHandle;
}

bgfx::TextureHandle Texture2D::GetHandle() const
{
	return _handle;
}

Texture2D::~Texture2D()
{
}
