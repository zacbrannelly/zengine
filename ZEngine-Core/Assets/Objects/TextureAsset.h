#pragma once

#include "../../Rendering/Texture2D.h"
#include "../Asset.h"
#include <bgfx/bgfx.h>

class TextureAsset : public Asset
{
public:
	TextureAsset(std::string name);
	~TextureAsset();

	bool Load(std::string path) override;
	void Release() override;

	bool LoadTexture(uint64_t flags = BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC);

	Texture2D* GetTexture() const;

	static Asset* CreateInstance(std::string name);

	static ObjectType GetStaticType()
	{
		return TEXTURE_ASSET;
	}
private:
	Texture2D* _texture;
	unsigned char* _imageData;
	unsigned int _imageSize;
	int _width, _height;
};

