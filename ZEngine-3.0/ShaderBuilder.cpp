#include "ShaderBuilder.h"
#include "File.h"

#include <bgfx/bgfx.h>
#include <bgfx/brtshaderc.h>

bool ShaderBuilder::BuildToFile(ShaderBuilderType type, const std::string& path, const std::string& varyingDefPath)
{
	// Attempt to build the shader
	uint8_t* data = nullptr;
	int size = -1;
	auto result = Build(type, path, varyingDefPath, &data, &size);

	if (!result)
		return false;

	// Write to a file 
	File binOutput(path + ".bin");
	result = binOutput.WriteContent((const char*)data, size, true);

	// TODO: There may be a memory leak here!

	return result;
}

bool ShaderBuilder::Build(ShaderBuilderType type, const std::string& path, const std::string& varyingDefPath, uint8_t** data, int* size)
{
	auto shaderType = shaderc::ST_VERTEX;

	if (type == SB_FRAGMENT)
		shaderType = shaderc::ST_FRAGMENT;
	else if (type == SB_COMPUTE)
		shaderType = shaderc::ST_COMPUTE;

	auto result = shaderc::compileShader(shaderType, path.c_str(), nullptr, varyingDefPath.c_str());

	if (result == nullptr)
		return false;

	*data = result->data;
	*size = result->size;

	return true;
}
