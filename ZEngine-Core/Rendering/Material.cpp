#include "Material.h"
#include "Shader.h"
#include "Graphics.h"
#include "StandardShaders.h"

Material::Material(std::string name) : ZObject(name, ObjectType::MATERIAL)
{
	RegisterDerivedType(MATERIAL);
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
		_graphics->CreateUniform(name, bgfx::UniformType::Sampler, 1),
		{ bgfx::kInvalidHandle }
	};
}

void Material::SetUniform(const std::string& name, void* data, uint16_t numElements)
{
	auto iterator = _uniforms.find(name);
	if (iterator != _uniforms.end())
	{
		auto uniform = iterator->second;

		if (uniform.data != nullptr)
			delete uniform.data;

		uniform.data = data;
		uniform.numElements = numElements;
		_uniforms[name] = uniform;
	}
}

const Uniform& Material::GetUniform(const std::string& name)
{
	return _uniforms.find(name) != _uniforms.end() ? _uniforms[name] : Uniform();
}

void Material::SetTexture(const std::string& name, bgfx::TextureHandle texture)
{
	auto iterator = _textureSamplers.find(name);
	if (iterator != _textureSamplers.end())
	{
		auto sampler = iterator->second;
		sampler.texture = texture;
		_textureSamplers[name] = sampler;
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

void Material::Release()
{
	_shader = nullptr;

	for (auto pair : _textureSamplers)
	{
		bgfx::destroy(pair.second.handle);
	}

	_textureSamplers.clear();

	for (auto pair : _uniforms)
	{
		if (pair.second.data != nullptr)
			delete pair.second.data;

		bgfx::destroy(pair.second.handle);
	}

	_uniforms.clear();
}

ZObject* Material::CreateInstance(std::string name, ObjectType type)
{
	return new Material(name);
}

ZObject* Material::CreateDefaultInstance(std::string name, ObjectType type)
{
	auto newInstance = new Material(name);

	// TODO: Replace with a lit shader.
	newInstance->SetShader(StandardShaders::GetUnlitColorShader());

	return newInstance;
}

Material::~Material()
{
	for (auto pair : _uniforms)
	{
		if (pair.second.data != nullptr)
			delete pair.second.data;
	}
}
