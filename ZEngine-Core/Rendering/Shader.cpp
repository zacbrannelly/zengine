#include "Shader.h"
#include "../Logging/LoggingSystem.h"
#include <fstream>

using namespace std;
using namespace ZEngine;

static const std::string MODULE_NAME = "Shader";

Shader::Shader(string name) : ZObject(name, SHADER)
{
	RegisterDerivedType(SHADER);
}

ZObject* Shader::CreateInstance(string name, ObjectType type)
{
	return new Shader(name);
}

bool Shader::Load(std::string vertPath, std::string fragPath, uint64_t state, int pass)
{
	auto vertexShaderData = LoadFile(vertPath);
	
	if (vertexShaderData == nullptr)
	{
		LoggingSystem::GetInstance()->LogError("Load: Failed to load vertex shader from file: " + vertPath, MODULE_NAME);
		return false;
	}

	auto fragShaderData = LoadFile(fragPath);

	if (fragShaderData == nullptr)
	{
		LoggingSystem::GetInstance()->LogError("Load: Failed to load fragment shader from file: " + fragPath, MODULE_NAME);
		return false;
	}

	return Load(
		vertexShaderData->data,
		vertexShaderData->size,
		fragShaderData->data,
		fragShaderData->size,
		state,
		pass
	);
}

void Shader::LoadUniforms(const bgfx::ShaderHandle& shader)
{
	// Fetch the uniforms 
	bgfx::UniformHandle foundUniforms[256];
	uint16_t numUniforms = bgfx::getShaderUniforms(shader, foundUniforms, 256);
	for (uint16_t i = 0; i < numUniforms; ++i)
	{
		bgfx::UniformHandle uniform = foundUniforms[i];
		bgfx::UniformInfo info;
		bgfx::getUniformInfo(uniform, info);
		_uniforms.push_back({ info.name, uniform, info.type, info.num });
	}
}

bool Shader::Load(uint8_t* vertData, uint32_t vertSize, uint8_t* fragData, uint32_t fragSize, uint64_t state, int pass)
{
	const bgfx::Memory* vertexShaderData = bgfx::makeRef(vertData, vertSize);
	const bgfx::Memory* fragShaderData = bgfx::makeRef(fragData, fragSize);

	auto vertexShader = bgfx::createShader(vertexShaderData);
	auto fragmentShader = bgfx::createShader(fragShaderData);
	auto program = bgfx::createProgram(vertexShader, fragmentShader, true);

	if (program.idx != bgfx::kInvalidHandle)
	{
		// Fetch the uniforms 
		LoadUniforms(vertexShader);
		LoadUniforms(fragmentShader);

		// Either set the pass to an existing, or create a new one
		if (_pass.size() == 0 || pass >= _pass.size())
		{
			_pass.push_back({ vertexShader, fragmentShader, program, state });
		}
		else
			_pass[pass] = { vertexShader, fragmentShader, program, state };
	}
	else
		return false;
	
	return true;
}

void Shader::Release()
{
	// Destroy any previous shader handles
	for (auto& pass : _pass)
	{
		if (pass.program.idx != bgfx::kInvalidHandle)
			bgfx::destroy(pass.program);
	}

	// Remove all previous passes
	_pass.clear();
}

const bgfx::ProgramHandle& Shader::GetHandle(int pass) const
{
	return _pass[pass].program;
}

const bgfx::ShaderHandle & Shader::GetVertexShader(int pass) const
{
	return _pass[pass].vertexShader;
}

const bgfx::ShaderHandle & Shader::GetFragmentShader(int pass) const
{
	return _pass[pass].fragmentShader;
}

const std::vector<Pass>& Shader::GetPasses() const
{
	return _pass;
}

const std::vector<UniformInfo>& Shader::GetUniforms() const
{
	return _uniforms;
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
	for (auto& pass : _pass)
	{
		bgfx::destroy(pass.program);
	}
}
