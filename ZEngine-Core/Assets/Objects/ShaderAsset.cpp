#include "ShaderAsset.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "../../Utilities/Directory.h"

using namespace std;
using namespace nlohmann;
using namespace ZEngine;

ShaderAsset::ShaderAsset(std::string name) : Asset(name, ObjectType::SHADER_ASSET)
{
	RegisterDerivedType(SHADER_ASSET);
	_shader = nullptr;
}

bool ShaderAsset::Load(std::string path)
{
	ifstream in(path, ios::in);

	if (!in.is_open())
		return false;

	json root;
	root << in;

	in.close();

	auto assetDir = Directory::GetBasePath(path);
	if (assetDir[assetDir.size() - 1] != '/')
		assetDir += '/';

	string name;

	auto it = root.find("name");
	if (it != root.end())
	{
		name = it.value().get<std::string>();
		cout << "Loading shader asset: " << name << endl;
	}
	else
		return false;

	if (_shader == nullptr)
		_shader = new Shader(name);
	else
		_shader->Release();

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

				auto vertexPath = assetDir + passObject.at("vertex").get<std::string>();
				auto fragPath = assetDir + passObject.at("fragment").get<std::string>();

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

ZObject* ShaderAsset::CreateInstance(std::string name, ObjectType type)
{
	return new ShaderAsset(name);
}

ShaderAsset::~ShaderAsset()
{
}
