#include "Asset.h"
#include <fstream>
#include "../Logging/LoggingSystem.h"

using namespace ZEngine;

Asset::Asset(std::string name, ObjectType type) : ZObject(name, type)
{
	RegisterDerivedType(ASSET);
	_isLoaded = false;
}

bool Asset::LoadBinary(const std::string& path, char* result, unsigned int& resultSize)
{
	std::ifstream in(path, std::ios::in | std::ios::binary | std::ios::ate);

	if (in.is_open())
	{
		std::ifstream::pos_type size = in.tellg();
		in.seekg(0, std::ios::beg);

		unsigned int allocSize = (unsigned int)size + 1;

		result = new char[allocSize];
		resultSize = allocSize;

		in.read(result, resultSize);

		// Null terminate (just in case)
		result[allocSize - 1] = '\0';
	}
	else
	{
		LoggingSystem::GetInstance()->LogError("LoadBinary: Failed to open file at path: " + path, "Asset");
		return false;
	}

	in.close();

	return true;
}

bool Asset::LoadText(const std::string& path, std::string& result)
{
	std::ifstream in(path, std::ios::in);

	if (in.is_open())
	{
		result = std::string((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
	}
	else
	{
		LoggingSystem::GetInstance()->LogError("LoadText: Failed to open file at path: " + path, "Asset");
		return false;
	}

	in.close();
	
	return true;
}

void Asset::SetPath(std::string path)
{
	_path = path;
}

std::string Asset::GetPath() const
{
	return _path;
}

void Asset::SetLoaded(bool loaded)
{
	_isLoaded = loaded;
}

bool Asset::IsLoaded() const
{
	return _isLoaded;
}

void Asset::Release()
{
}

Asset::~Asset()
{
	if (IsLoaded())
		Release();
}
