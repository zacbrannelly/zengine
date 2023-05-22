#include "MaterialBuilder.h"
#include "../Windows/MaterialEditor.h"
#include "../Utilities/File.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;

bool MaterialBuilder::BuildToFile(const std::string& path, const MaterialEditorData& data)
{
	json root;

	root["name"] = data.name;
	root["shader"] = data.shaderID;
	
	auto& textures = root["textures"] = json::array_t();
	auto& uniforms = root["uniforms"] = json::array_t();

	for (const auto& samplerPair : data.samplers)
	{
		json::object_t samplerObj;
		samplerObj["name"] = samplerPair.first;

		if (samplerPair.second.assetID < 0 && samplerPair.second.assetPath != "None")
			samplerObj["path"] = samplerPair.second.assetPath;
		else if (samplerPair.second.assetID >= 0)
			samplerObj["id"] = samplerPair.second.assetID;
		else
			return false;

		textures.push_back(samplerObj);
	}

	for (const auto& uniformPair : data.uniforms)
	{
		json::object_t uniformObj;
		uniformObj["name"] = uniformPair.first;
		uniformObj["type"] = uniformPair.second.type;
		uniformObj["numElements"] = uniformPair.second.numElements;

		int num = 4;
		switch (uniformPair.second.type)
		{
		case bgfx::UniformType::Mat4:
			num = 16;
			break;
		case bgfx::UniformType::Mat3:
			num = 9;
			break;
		}

		uniformObj["values"] = json::array_t();

		for (int i = 0; i < num * uniformPair.second.numElements; i++)
		{
			uniformObj["values"].push_back(((float*)uniformPair.second.data)[i]);
		}

		uniforms.push_back(uniformObj);
	}

	// Serialize JSON to string
	auto result = root.dump(4);

	// Write to the asset file
	File output(path);
	return output.WriteContent(&result[0], result.size());
}