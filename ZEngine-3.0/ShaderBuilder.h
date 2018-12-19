#pragma once

#include <string>

enum ShaderBuilderType
{
	SB_VERTEX, SB_FRAGMENT, SB_COMPUTE
};

class ShaderBuilder
{
public:
	static bool Build(ShaderBuilderType type, const std::string& path, const std::string& varyingDefPath, uint8_t** data, int* size);
	static bool BuildToFile(ShaderBuilderType type, const std::string& path, const std::string& varyingDefPath);
};
