#pragma once

#include "../Asset.h"
#include "../../Rendering/Shader.h"

class Shader;

class ShaderAsset : public Asset
{
public:
	ShaderAsset(std::string name);
	~ShaderAsset();

	bool Load(std::string path) override;
	void Release() override;

	Shader* GetShader() const;

	static Asset* CreateInstance(std::string name);

private:
	Shader* _shader;
};

