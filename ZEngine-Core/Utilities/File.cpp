#include "File.h"
#include "Directory.h"

#include <stdexcept>
#include <fstream>

using namespace std;
using namespace ZEngine;

File::File(string path)
{
	_path = Directory::ConvertPath(path);
	_filename = Directory::GetFilename(path);
}

void File::Set(string path)
{
	_path = Directory::ConvertPath(path);
	_filename = Directory::GetFilename(path);
}

bool File::Create()
{
	ofstream out(_path, ios::out);

	if (!out.is_open())
		return false;

	out.close();

	return true;
}

bool File::Copy(std::string newPath)
{
	ifstream in(_path, ios::binary);
	ofstream out(newPath, ios::binary);

	out << in.rdbuf();
}

bool File::Move(std::string newPath)
{
	ifstream in(_path, ios::binary);
	ofstream out(_path, ios::binary);

	if (!in.good() || !out.good())
		return false;

	out << in.rdbuf();

	in.close();
	out.close();

	if (remove(_path.c_str()) != 0)
		return false;

	_path = newPath;

	return true;
}

bool File::Delete()
{
	return remove(_path.c_str()) == 0;
}

bool File::ReadContent(char* buffer, int bufferLen, bool isBinary) const
{
	ifstream in(_path, ios::in | ios::ate | (isBinary ? ios::binary : 0));

	if (!in.is_open())
		return false;

	int length = in.tellg();

	if (length > bufferLen)
		return false;

	in.seekg(0, in.beg);

	memset(buffer, '\0', bufferLen);
	in.read(buffer, length);

	in.close();

	return true;
}

bool File::WriteContent(const char* buffer, int bufferLen, bool isBinary) const
{
	ofstream out(_path, ios::out | (isBinary ? ios::binary : 0));

	if (!out.is_open())
	{
		return false;
	}

	out.write(buffer, bufferLen);

	out.close();

	return true;
}

nlohmann::json File::ReadJson() const
{
	ifstream in(_path);
	nlohmann::json json;

	if (!in.is_open())
		throw runtime_error("Failed to read json, file not found");

	in >> json;
	in.close();

	return json;
}

bool File::WriteJson(const nlohmann::json& json) const
{
	ofstream out(_path);

	if (!out.is_open())
		return false;

	out << json.dump(2);

	out.close();

	return true;
}

bool File::Exists() const
{
	ifstream in(_path);

	if (in.good())
	{
		in.close();
		return true;
	}

	return false;
}

const string& File::GetFilename() const
{
	return _filename;
}

const string& File::GetPath() const
{
	return _path;
}

string File::GetRelativePath(string to) const
{
	Directory dir(Directory::GetBasePath(_path));
	auto relativeBase = dir.GetPathRelativeTo(to);
	
	if (relativeBase[relativeBase.size() - 1] != '/')
		relativeBase += '/';
	
	return relativeBase + _filename;
}

std::string File::GetName() const
{
	auto ext = GetExtension();
	return _filename.substr(0, _filename.size() - ext.size() - 1);
}

string File::GetExtension() const
{
	return Directory::GetExtension(_path);
}

string File::GetDirectory() const 
{
	return Directory::GetBasePath(_path);
}

File::~File()
{

}
