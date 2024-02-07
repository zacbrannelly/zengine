#include "MaterialBuilder.h"
#include "../Windows/MaterialEditor.h"
#include <ZEngine-Core/Utilities/File.h>
#include <ZEngine-Core/Utilities/Directory.h>

#include <nlohmann/json.hpp>

using namespace nlohmann;

bool MaterialBuilder::BuildToFile(const std::string& path, const MaterialEditorData& data)
{
	json root;

	root["name"] = data.name;
	root["shader"] = uuids::to_string(data.shaderID);
	
	auto& textures = root["textures"] = json::array_t();
	auto& uniforms = root["uniforms"] = json::array_t();

	auto assetDir = Directory::GetBasePath(path);

	for (const auto& samplerPair : data.samplers)
	{
		json::object_t samplerObj;
		samplerObj["name"] = samplerPair.first;

		if (samplerPair.second.assetID.is_nil() && samplerPair.second.assetPath != "None")
			samplerObj["path"] = samplerPair.second.assetPath.substr(assetDir.size());
		else if (!samplerPair.second.assetID.is_nil())
			samplerObj["id"] = uuids::to_string(samplerPair.second.assetID);
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

		for (int i = 0; i < num * uniformPair.second.numElements; i += num)
		{
			json::array_t instance;
			for (int j = 0; j < num; j++)
				instance.push_back(((float*)uniformPair.second.data)[i + j]);
			uniformObj["values"].push_back(instance);
		}

		uniforms.push_back(uniformObj);
	}

	// Serialize JSON to string
	auto result = root.dump(4);

	// Write to the asset file
	File output(path);
	return output.WriteContent(&result[0], result.size());
}