#pragma once

#include <bgfx/bgfx.h>
#include <string>

class Texture2D
{
public:
	Texture2D();
	~Texture2D();

	bool Load(std::string path);

	bgfx::TextureHandle GetHandle() const;

private:
	bgfx::TextureHandle _handle;
};

