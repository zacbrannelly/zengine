#include "ShaderAsset.h"


ShaderAsset::ShaderAsset(std::string name) : Asset(name, ObjectType::SHADER_ASSET, BINARY)
{
}

bool ShaderAsset::Load(std::string path)
{
	//TODO: Implement the load function for ShaderAsset
	return false;
}

void ShaderAsset::Release()
{
	if (_shader != nullptr)
		delete _shader;
}

Shader* ShaderAsset::GetShader() const
{
	return _shader;
}

Asset* ShaderAsset::CreateInstance(std::string name)
{
	return new ShaderAsset(name);
}

ShaderAsset::~ShaderAsset()
{
}
