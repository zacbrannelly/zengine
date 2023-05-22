#include "ShaderBuilder.h"
#include "../Utilities/File.h"

#include <sstream>
#include <bx/process.h>
#include <bgfx/bgfx.h>

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
	// TODO: Make a subprocess call to the `shaderc` executable to build the shader.
	// Getting Brtshaderc working with CMake is proving more difficult than its worth.

	bx::ProcessReader reader;

#if BX_PLATFORM_WINDOWS
	std::string platform = "windows";
	std::string profile = "opengl";
#elif BX_PLATFORM_LINUX
	std::string platform = "linux";
	std::string profile = "opengl";
#elif BX_PLATFORM_OSX
	std::string platform = "osx";
	std::string profile = "metal";
#endif

	std::string typeArgValue;
	switch (type) {
	case SB_VERTEX:
		typeArgValue = "vertex";
		break;
	case SB_FRAGMENT:
		typeArgValue = "fragment";
		break;
	case SB_COMPUTE:
		typeArgValue = "compute";
		break;
	default:
		throw std::runtime_error("Invalid shader type");
	}

	std::string args;
	args += "-f " + path + " ";
	args += "--varyingdef " + varyingDefPath + " ";
	args += "-o " + path + ".bin" + " ";
	args += "--platform " + platform + " ";
	args += "--type " + typeArgValue + " ";
	args += "--profile " + profile;

	bx::Error error;
	std::stringbuf outputBuffer;

	if (reader.open("./bin/shaderc", args.c_str(), &error)) {
		char buffer[1024];
		while (reader.read(buffer, sizeof(buffer), &error) > 0) {
			outputBuffer.sputn(buffer, sizeof(buffer));
		}
		reader.close();
	} else {
		return false;
	}

	const char* output = new char[outputBuffer.str().length() + 1];
	strcpy((char*)output, outputBuffer.str().c_str());

	*data = (uint8_t*)output;
	*size = outputBuffer.str().length();

	return true;
}
