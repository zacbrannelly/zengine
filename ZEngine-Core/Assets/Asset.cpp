#include "Asset.h"
#include <iostream>
#include <fstream>

using namespace std;

Asset::Asset(string name, ObjectType type) : ZObject(name, type)
{
	_isLoaded = false;
}

bool Asset::LoadBinary(const string& path, char* result, unsigned int& resultSize)
{
	ifstream in(path, ios::in | ios::binary | ios::ate);

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
		cout << "ASSET: Failed to open file at path: " << path << endl;
		return false;
	}

	in.close();

	return true;
}

bool Asset::LoadText(const string& path, string& result)
{
	ifstream in(path, ios::in);

	if (in.is_open())
	{
		result = string((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
	}
	else
	{
		cout << "ASSET: Failed to open file at path: " << path << endl;
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
