#include "ShaderAsset.h"
#include "../../json/json.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using namespace nlohmann;

ShaderAsset::ShaderAsset(std::string name) : Asset(name, ObjectType::SHADER_ASSET)
{
}

bool ShaderAsset::Load(std::string path)
{
	ifstream in(path, ios::in);

	if (!in.is_open())
		return false;

	json root;
	root << in;

	in.close();

	string name;

	auto it = root.find("name");
	if (it != root.end())
	{
		name = it.value().get<std::string>();
		cout << "Loading shader asset: " << name << endl;
	}
	else
		return false;

	_shader = new Shader(name);

	// Get passes
	it = root.find("pass");
	if (it != root.end() && it.value().is_array())
	{
		auto passes = it.value().get<json::array_t>();
		int numPasses = passes.size();

		if (numPasses == 0)
		{
			cout << "SHADER_ASSET: Failed to load the shader '" << name << "' since it has no pass!" << endl;
			return false;
		}

		for (int i = 0; i < numPasses; i++)
		{
			if (passes[i].is_object())
			{
				auto passObject = passes[i];

				auto vertexPath = passObject.at("vertex").get<std::string>();
				auto fragPath = passObject.at("fragment").get<std::string>();

				// TODO: Get the fkn states from the 
				if (passObject.find("states") != passObject.end())
				{
					auto states = passObject.at("states");
				}

				// Load the pass into the shader
				if (!_shader->Load(vertexPath, fragPath, 0, i))
				{
					cout << "SHADER_ASSET: Failed to load a shader from pass: " << i << endl;
					return false;
				}
			}
		}
	}

	SetPath(path);
	SetLoaded(true);

	return true;
}

void ShaderAsset::Release()
{
	if (_shader != nullptr)
		delete _shader;
}

Shader* ShaderAsset::GetShader() const
{
	return _shader;
}

Asset* ShaderAsset::CreateInstance(std::string name)
{
	return new ShaderAsset(name);
}

ShaderAsset::~ShaderAsset()
{
}
