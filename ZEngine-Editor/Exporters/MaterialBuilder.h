#pragma once

#include <string>
struct MaterialEditorData;

class MaterialBuilder
{
public:
	static bool BuildToFile(const std::string& path, const MaterialEditorData& data);
};
