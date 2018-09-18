#pragma once

#include <bgfx/bgfx.h>
#include <string>

class Texture2D
{
public:
	Texture2D(uint16_t width, uint16_t height, bool hasMips, uint16_t numLayers, bgfx::TextureFormat::Enum format, uint64_t flags, void* data = nullptr, uint32_t size = 0);
	~Texture2D();

	bool IsValid() const;
	bgfx::TextureHandle GetHandle() const;

private:
	bgfx::TextureHandle _handle;
};

