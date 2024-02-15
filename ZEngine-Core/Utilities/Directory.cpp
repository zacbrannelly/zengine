#include "Directory.h"
#include "File.h"

#include "../Misc/StringUtil.h"
#include <string>
#include <algorithm>
#include <filesystem>

using namespace std;

Directory::Directory(string path)
{
	_fullpath = ConvertPath(path);
	_fullpath = AddTrailingSlash(_fullpath);
	_name = GetFilename(_fullpath);
}

void Directory::Set(string path)
{
	_fullpath = ConvertPath(path);
	_fullpath = AddTrailingSlash(_fullpath);
	_name = GetFilename(_fullpath);
}

const string& Directory::GetName() const
{
	return _name;
}

const string& Directory::GetPath() const
{
	return _fullpath;
}

File Directory::FindFile(string filename) const
{
	return File(FileExists(filename) ? string(_fullpath + filename) : "");
}

Directory Directory::FindDirectory(string directoryName) const
{
	return Directory(DirectoryExists(directoryName) ? _fullpath + directoryName : "");
}

Directory Directory::GetParentDirectory() const
{
	auto delim = GetPathDelimiter(_fullpath);

	// Remove trailing slash
	auto path = _fullpath.substr(0, _fullpath.size() - 1);

	auto index = path.find_last_of(delim);
	path = path.substr(0, index);

	return Directory(path);
}

char Directory::GetPathDelimiter(string fullpath)
{
	char separator = '/';

	if (!StringUtil::Contains(fullpath, "/"))
	{
		if (StringUtil::Contains(fullpath, "\\"))
		{
			// Has backslashes
			separator = '\\';
		}
		else
		{
			// No slashes so invalid
			separator = '\0';
		}
	}

	return separator;
}

string Directory::GetFilename(string fullpath)
{
	auto delim = GetPathDelimiter(fullpath);

	if (delim != '\0')
	{
		auto data = StringUtil::Split(fullpath, delim);
		return data.back();
	}
	
	return fullpath;
}

string Directory::GetExtension(string fullpath)
{
	auto filename = GetFilename(fullpath);
	auto data = StringUtil::Split(filename, '.');

	if (data.size() == 0 || data.size() == 1)
	{
		return string("");
	}
	
	return data.back();
}

string Directory::GetBasePath(string fullpath)
{
	char delim = GetPathDelimiter(fullpath);

	if (delim != '\0')
	{
		auto index = fullpath.find_last_of(delim);
		return fullpath.substr(0, index + 1);
	}

	return "./";
}

std::string Directory::AddTrailingSlash(std::string path) const
{
	if (path[path.size() - 1] != '/')
	{
		path += '/';
	}
	return path;
}

std::string Directory::ConvertPath(std::string path)
{
	// Convert all backslashes to forward slashes
	transform(path.begin(), path.end(), path.begin(), 
	[](auto c)
	{ 
		if (c == '\\')
			return '/'; 

		return c; 
	});

	return path;
}

string Directory::ConvertPathDelimiter(std::string path, char delim)
{
	auto currDelim = GetPathDelimiter(path);

	transform(path.begin(), path.end(), path.begin(),
	[&delim, &currDelim](auto c)
	{
		if (c == currDelim)
			return delim;
		
		return c;
	});

	return path;
}

string Directory::GetCurrentWorkingDirectory()
{
	return filesystem::current_path().string();	
}

string Directory::GetAbsolutePath(std::string path)
{
	return Directory(path).GetAbsolutePath();
}

Directory::~Directory()
{
}
