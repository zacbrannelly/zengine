#include "Material.h"
#include "Shader.h"
#include "Graphics.h"

Material::Material(std::string name) : ZObject(name, ObjectType::MATERIAL)
{
	_shader = nullptr;
	_graphics = Graphics::GetInstance();
}

void Material::RegisterUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t numElements)
{
	Uniform uniform = 
	{
		_graphics->CreateUniform(name, type, numElements),
		type,
		numElements,
		nullptr
	};

	_uniforms[name] = uniform;
}

void Material::RegisterSampler(const std::string& name)
{
	_textureSamplers[name] = 
	{
		_graphics->CreateUniform(name, bgfx::UniformType::Int1, 1),
		{ bgfx::kInvalidHandle }
	};
}

void Material::SetUniform(const std::string& name, void* data, uint16_t numElements)
{
	auto iterator = _uniforms.find(name);
	if (iterator != _uniforms.end())
	{
		auto uniform = iterator->second;

		uniform.data = data;
		uniform.numElements = numElements;

		_uniforms.insert_or_assign(iterator, name, uniform);
	}
}

void Material::SetTexture(const std::string& name, bgfx::TextureHandle texture)
{
	auto iterator = _textureSamplers.find(name);
	if (iterator != _textureSamplers.end())
	{
		auto sampler = iterator->second;
		sampler.texture = texture;

		_textureSamplers.insert_or_assign(iterator, name, sampler);
	}
}

void Material::SetShader(Shader* shader)
{
	_shader = shader;
}

Shader* Material::GetShader() const
{
	return _shader;
}

const std::map<std::string, Uniform>& Material::GetUniforms() const
{
	return _uniforms;
}

const std::map<std::string, Sampler> Material::GetSamplers() const
{
	return _textureSamplers;
}

void Material::Apply()
{
	for (auto pair : _uniforms)
	{
		auto uniform = pair.second;
		_graphics->SetUniform(uniform.handle, uniform.data, uniform.numElements);
	}

	// TODO: Check the bounds of the count int
	uint8_t count = 0;
	for (auto it = _textureSamplers.begin(); it != _textureSamplers.end(); it++, count++)
	{
		auto sampler = it->second;
		_graphics->SetTexture(count, sampler.handle, sampler.texture);
	}
}

ZObject* Material::CreateInstance(std::string name, ObjectType type)
{
	return new Material(name);
}

Material::~Material()
{
	for (auto pair : _uniforms)
	{
		if (pair.second.data != nullptr)
			delete pair.second.data;
	}
}