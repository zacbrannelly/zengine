#pragma once

#include "../Map/Objects/ZObject.h"
#include <bgfx/bgfx.h>
#include <map>

class Shader;

struct Uniform
{
	bgfx::UniformHandle handle;
	bgfx::UniformType::Enum type;
	int numElements;

	void* data;
};

class Material : public ZObject
{
public:
	Material(std::string name);
	~Material();

	void RegisterUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t numElements);
	void SetUniform(const std::string& name, void* data, uint16_t numElements);
	void Apply();

	void SetShader(Shader* shader);
	Shader* GetShader() const;

	const std::map<std::string, Uniform>& GetUniforms() const;

private:
	std::map<std::string, Uniform> _uniforms;
	Shader* _shader;

public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
};

