#include "Material.h"
#include "Shader.h"


Material::Material(std::string name) : ZObject(name, ObjectType::MATERIAL)
{
	_shader = nullptr;
}

void Material::RegisterUniform(const std::string& name, bgfx::UniformType::Enum type, uint16_t numElements)
{
	Uniform uniform = 
	{
		bgfx::createUniform(name.c_str(), type, numElements),
		type,
		numElements,
		nullptr
	};

	_uniforms[name] = uniform;
}

void Material::SetUniform(const std::string& name, void* data, uint16_t numElements)
{
	auto iterator = _uniforms.find(name);
	if (iterator != _uniforms.end())
	{
		auto uniform = iterator->second;

		uniform.data = data;
		uniform.numElements = numElements;

		_uniforms[name] = uniform;
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

void Material::Apply()
{
	if (_shader == nullptr) return;

	for (auto pair : _uniforms)
	{
		auto uniform = pair.second;
		_shader->SetUniform(uniform.handle, uniform.data, uniform.numElements);
	}
}

ZObject* Material::CreateInstance(std::string name, ObjectType type)
{
	return new Material(name);
}

Material::~Material()
{
}
