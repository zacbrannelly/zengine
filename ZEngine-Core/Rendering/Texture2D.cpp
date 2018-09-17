#include "Texture2D.h"

#include "../../stb/stb_image.h"

Texture2D::Texture2D()
{
}

bool Texture2D::Load(std::string path)
{
	int width, height, channel;
	auto data = stbi_load(path.c_str(), &width, &height, &channel, 4);

	if (data == nullptr)
		return false;
	
	bgfx::TextureInfo info;
	bgfx::calcTextureSize(info, width, height, 0, false, false, 1, bgfx::TextureFormat::RGBA8);

	uint64_t flags = BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC;
	_handle = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::RGBA8, flags, bgfx::makeRef(data, info.storageSize));

	//delete data;

	return _handle.idx != bgfx::kInvalidHandle;
}

bgfx::TextureHandle Texture2D::GetHandle() const
{
	return _handle;
}

Texture2D::~Texture2D()
{
}
