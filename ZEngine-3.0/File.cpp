#include "File.h"
#include "Directory.h"

#include <fstream>

using namespace std;

File::File(string path)
{
	_path = Directory::ConvertPath(path);
	_filename = Directory::GetFilename(path);
}

bool File::Create()
{
	ofstream out(_path, ios::out);

	if (out.is_open())
		return false;

	out.close();

	return true;
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

File::~File()
{

}
