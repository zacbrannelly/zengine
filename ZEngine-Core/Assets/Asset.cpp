#include "Asset.h"
#include <iostream>
#include <fstream>

using namespace std;

Asset::Asset(string name, ObjectType type, AssetFileFormat format) : ZObject(name, type)
{
	_data = nullptr;
	_size = 0;
	_format = format;
}

bool Asset::Load(string path)
{
	switch (_format)
	{
	case TEXT:
		return LoadText(path);
	case BINARY:
		return LoadBinary(path);
	}

	return false;
}

bool Asset::LoadBinary(string& path)
{
	ifstream in(path, ios::in | ios::binary | ios::ate);

	if (in.is_open())
	{
		std::ifstream::pos_type size = in.tellg();
		in.seekg(0, std::ios::beg);

		unsigned int allocSize = (unsigned int)size + 1;

		_data = new char[allocSize];
		_size = allocSize;
		_path = path;

		in.read((char*)_data, size);

		// Null terminate & set size 
		((char*)_data)[allocSize - 1] = '\0';
	}
	else
	{
		cout << "ASSET: Failed to open file at path: " << path << endl;
		return false;
	}

	in.close();

	return IsLoaded();
}

bool Asset::LoadText(string& path)
{
	ifstream in(path, ios::in);

	if (in.is_open())
	{
		string* code = new string((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));

		// Set the string to the pointer data
		_data = code;
		_size = code->size();
		_path = path;
	}
	else
	{
		cout << "ASSET: Failed to open file at path: " << path << endl;
		return false;
	}

	in.close();
	
	return IsLoaded();
}

void Asset::SetData(void* data)
{
	_data = data;
}

void* Asset::GetData() const
{
	return _data;
}

string Asset::GetDataString() const
{
	return std::string((const char*)_data, _size);
}

void Asset::SetPath(std::string path)
{
	_path = path;
}

std::string Asset::GetPath() const
{
	return _path;
}

void Asset::SetDataSize(unsigned int size)
{
	_size = size;
}

unsigned int Asset::GetDataSize() const
{
	return _size;
}

void Asset::Release()
{
	if (_data == nullptr) return;

	delete[] _data;

	_data = nullptr;
	_size = 0;
}

bool Asset::IsLoaded() const
{
	return _data != nullptr && _size > 0;
}

Asset::~Asset()
{
	if (IsLoaded())
		Release();
}
