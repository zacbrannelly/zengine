#include "TextureAsset.h"
#include "stb_image.h"
#include <iostream>

using namespace std;

TextureAsset::TextureAsset(std::string name) : Asset(name, ObjectType::TEXTURE_ASSET)
{
	RegisterDerivedType(TEXTURE_ASSET);
	_imageData = nullptr;
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
		_imageData = imageData;
		_imageSize = info.storageSize;

		// Save the path
		SetPath(path);

		// Load the texture into the graphics system.
		LoadTexture();

		return true;
	}
	else
		std::cout << "TEXTURE_ASSET: Failed to load the image: " << path << endl;

	return false;
}

bool TextureAsset::LoadTexture(uint64_t flags)
{
	if (_imageData == nullptr) return false;

	_texture = new Texture2D(_width, _height, false, 1, bgfx::TextureFormat::RGBA8, flags, _imageData, _imageSize);

	// Set the texture as loaded
	SetLoaded(_texture->IsValid());

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
	if (_texture != nullptr)
		delete _texture;

	if (_imageData != nullptr)
		delete[] _imageData;
}

TextureAsset::~TextureAsset()
{
}
