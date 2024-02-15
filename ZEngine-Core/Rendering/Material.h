#pragma once

#include "../Map/Objects/ZObject.h"
#include <bgfx/bgfx.h>
#include <map>

class Shader;
class Graphics;

struct Uniform
{
	bgfx::UniformHandle handle;
	bgfx::UniformType::Enum type;
	int numElements;
	void* data;
};

struct Sampler
{
	bgfx::UniformHandle handle;
	bgfx::TextureHandle texture;
};

class Material : public ZObject
{
public:
	Material(std::string name);
	~Material();

	void RegisterUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t numElements);
	void RegisterSampler(const std::string& name);

	void SetUniform(const std::string& name, void* data, uint16_t numElements);
	const Uniform& GetUniform(const std::string& name);

	void SetTexture(const std::string& name, bgfx::TextureHandle texture);

	void Apply();
	void Release();

	void SetShader(Shader* shader);
	Shader* GetShader() const;

	const std::map<std::string, Uniform>& GetUniforms() const;
	const std::map<std::string, Sampler> GetSamplers() const;
private:
	std::map<std::string, Uniform> _uniforms;
	std::map<std::string, Sampler> _textureSamplers;
	Shader* _shader;
	Graphics* _graphics;

public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
	static ZObject* CreateDefaultInstance(std::string name, ObjectType type);

	static ObjectType GetStaticType()
	{
		return MATERIAL;
	}
};

