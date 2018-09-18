#include "TextureAsset.h"
#include "../../../stb/stb_image.h"

using namespace std;

TextureAsset::TextureAsset(std::string name) : Asset(name, ObjectType::TEXTURE_ASSET, BINARY)
{
}

bool TextureAsset::Load(string path)
{
	// Load the image using stb_image
	int channels;
	auto imageData = stbi_load(path.c_str(), &_width, &_height, &channels, 4);

	if (imageData != nullptr)
	{
		// Calculate the size of the image in bytes
		bgfx::TextureInfo info;
		bgfx::calcTextureSize(info, _width, _height, 0, false, false, 1, bgfx::TextureFormat::RGBA8);

		// Set the data
		SetData(imageData);
		SetDataSize(info.storageSize);
		SetPath(path);

		return IsLoaded();
	}

	return false;
}

bool TextureAsset::LoadTexture(uint64_t flags)
{
	_texture = new Texture2D(_width, _height, false, 1, bgfx::TextureFormat::RGBA8, flags, GetData(), GetDataSize());

	return _texture->IsValid();
}

Texture2D* TextureAsset::GetTexture() const
{
	return _texture;
}

Asset* TextureAsset::CreateInstance(std::string name)
{
	return new TextureAsset(name);
}

void TextureAsset::Release()
{
	Asset::Release();

	if (_texture != nullptr)
		delete _texture;
}

TextureAsset::~TextureAsset()
{
}
