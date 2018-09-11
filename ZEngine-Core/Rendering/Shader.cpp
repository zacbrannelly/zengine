#include "Shader.h"
#include <fstream>
#include <iostream>

using namespace std;

Shader::Shader(string name) : ZObject(name, ObjectType::SHADER)
{
}

ZObject* Shader::CreateInstance(string name, ObjectType type)
{
	return new Shader(name);
}

bool Shader::Load(std::string vertPath, std::string fragPath)
{
	auto vertexShaderData = LoadFile(vertPath);
	
	if (vertexShaderData == nullptr)
	{
		cout << "SHADER: Failed to load vertex shader from file: " << vertPath << endl;
		return false;
	}

	auto fragShaderData = LoadFile(fragPath);

	if (fragShaderData == nullptr)
	{
		cout << "SHADER: Failed to load frag shader from file: " << fragPath << endl;
		return false;
	}

	_vertexShader = bgfx::createShader(vertexShaderData);
	_fragmentShader = bgfx::createShader(fragShaderData);
	_program = bgfx::createProgram(_vertexShader, _fragmentShader, true);

	return _program.idx != bgfx::kInvalidHandle;
}

void Shader::SetUniform(const std::string & name, bgfx::UniformType::Enum type, const void * data, uint16_t numElements)
{
	auto& uniform = bgfx::createUniform(name.c_str(), type, numElements);
	bgfx::setUniform(uniform, data, numElements);
}

void Shader::SetUniform(bgfx::UniformHandle& uniform, const void* data, uint16_t numElements)
{
	bgfx::setUniform(uniform, data, numElements);
}

const bgfx::ProgramHandle& Shader::GetHandle() const
{
	return _program;
}

const bgfx::ShaderHandle & Shader::GetVertexShader() const
{
	return _vertexShader;
}

const bgfx::ShaderHandle & Shader::GetFragmentShader() const
{
	return _fragmentShader;
}

const bgfx::Memory* Shader::LoadFile(const std::string & path) const
{
	std::ifstream in(path.c_str(), std::ifstream::in | std::ifstream::binary | std::ifstream::ate);

	if (in.is_open())
	{
		std::ifstream::pos_type size = in.tellg();
		in.seekg(0, std::ios::beg);

		unsigned int allocSize = (unsigned int)size + 1;

		auto buffer = bgfx::alloc(allocSize);
		in.read((char*)buffer->data, size);

		in.close();

		// Null terminate
		buffer->data[allocSize - 1] = '\0';

		return buffer;
	}

	in.close();

	return nullptr;
}

Shader::~Shader()
{
	bgfx::destroy(_program);
}
