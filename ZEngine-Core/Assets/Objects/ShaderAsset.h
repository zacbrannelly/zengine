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

	static ZObject* CreateInstance(std::string name, ObjectType type);

	static ObjectType GetStaticType()
	{
		return SHADER_ASSET;
	}

private:
	Shader* _shader;
};

