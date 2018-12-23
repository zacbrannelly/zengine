#include "Directory.h"
#include "File.h"

#include <ZEngine-Core/Misc/StringUtil.h>
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <Shlwapi.h>

using namespace std;

Directory::Directory(string path)
{
	_fullpath = ConvertPath(path);
	_name = GetFilename(_fullpath);
}

void Directory::Set(string path)
{
	_fullpath = ConvertPath(path);
	_name = GetFilename(_fullpath);
}

bool Directory::Create()
{
	if (CreateDirectoryA(_fullpath.c_str(), nullptr))
	{
		cout << "DIRECTORY: Sucessfully created the directory '" << _fullpath << "'" << endl;
		return true;
	}
	else
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			cout << "DIRECTORY: Failed to create the directory '" << _fullpath << "' since it already exists!" << endl;
		}
		else
		{
			cout << "DIRECTORY: Failed to create the directory '" << _fullpath << "' since the path was invalid!" << endl;
		}

		return false;
	}
}

bool Directory::Move(string newPath)
{
	if (MoveFileExA(_fullpath.c_str(), newPath.c_str(), MOVEFILE_WRITE_THROUGH))
	{
		cout << "DIRECTORY: Sucessfully moved the directory '" << _fullpath << "'" << endl;
		_fullpath = newPath;

		return true;
	}
	else
	{
		cout << "DIRECTORY: Failed to move the directory '" << _fullpath << "' with error code: " << GetLastError() << endl;
		return false;
	}
}

bool Directory::Delete()
{
	if (RemoveDirectoryA(_fullpath.c_str()))
	{
		cout << "DIRECTORY: Successfully deleted directory '" << _fullpath << "'" << endl;
		return true;
	}
	else
	{
		cout << "DIRECTORY: Failed to delete the directory '" << _fullpath << "' with error code: " << GetLastError() << endl;
		return false;
	}
}

bool Directory::Exists() const
{
	auto attrib = GetFileAttributesA(_fullpath.c_str());

	if (attrib == INVALID_FILE_ATTRIBUTES)
		return false;

	return attrib & FILE_ATTRIBUTE_DIRECTORY;
}

bool Directory::IsEmpty() const
{
	WIN32_FIND_DATAA data;
	auto handle = FindFirstFileA((_fullpath + "/*").c_str(), &data);

	return handle == INVALID_HANDLE_VALUE;
}

const string& Directory::GetName() const
{
	return _name;
}

const string& Directory::GetPath() const
{
	return _fullpath;
}

string Directory::GetAbsolutePath() const
{
	char buffer[MAX_PATH] = "";
	GetFullPathNameA(_fullpath.c_str(), MAX_PATH, buffer, nullptr);

	return ConvertPath(string(buffer));
}

string Directory::GetPathRelativeTo(std::string path) const
{
	char buffer[MAX_PATH] = "";

	auto from = Directory(path).GetAbsolutePath();
	auto to = GetAbsolutePath();

	from = ConvertPathDelimiter(from, '\\');
	to = ConvertPathDelimiter(to, '\\');

	if (PathRelativePathToA(buffer, from.c_str(), FILE_ATTRIBUTE_DIRECTORY, to.c_str(), FILE_ATTRIBUTE_NORMAL))
	{
		return ConvertPath(string(buffer));
	}

	return "";
}

File Directory::FindFile(string filename) const
{
	return File(FileExists(filename) ? string(_fullpath + filename) : "");
}

Directory Directory::FindDirectory(string directoryName) const
{
	return Directory(DirectoryExists(directoryName) ? _fullpath + directoryName : "");
}

bool Directory::FileExists(string filename) const
{
	WIN32_FIND_DATAA data;
	auto handle = FindFirstFileA((_fullpath + filename).c_str(), &data);

	return handle != INVALID_HANDLE_VALUE && !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

bool Directory::DirectoryExists(string directoryName) const
{
	WIN32_FIND_DATAA data;
	auto handle = FindFirstFileA((_fullpath + directoryName).c_str(), &data);

	return handle != INVALID_HANDLE_VALUE && data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

std::vector<File> Directory::GetAllFilesWithExtention(std::string ext) const
{
	vector<File> results;

	WIN32_FIND_DATAA data;
	auto handle = FindFirstFileA((_fullpath + "*." + ext).c_str(), &data);

	if (handle != INVALID_HANDLE_VALUE && !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		results.push_back(File(_fullpath + data.cFileName));

	while (FindNextFileA(handle, &data))
	{
		if (handle != INVALID_HANDLE_VALUE && !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			results.push_back(File(_fullpath + data.cFileName));
	}

	return results;
}

std::vector<File> Directory::GetAllFiles() const
{
	vector<File> results;

	WIN32_FIND_DATAA data;
	auto handle = FindFirstFileA((_fullpath + "*").c_str(), &data);

	if (handle != INVALID_HANDLE_VALUE && !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		results.push_back(File(_fullpath + data.cFileName));

	while (FindNextFileA(handle, &data))
	{
		if (handle != INVALID_HANDLE_VALUE && !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			results.push_back(File(_fullpath + data.cFileName));
	}

	return results;
}

std::vector<Directory> Directory::GetAllDirectories() const
{
	vector<Directory> results;

	WIN32_FIND_DATAA data;
	auto handle = FindFirstFileA((_fullpath + "*").c_str(), &data);

	if (handle != INVALID_HANDLE_VALUE && data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		results.push_back(Directory(_fullpath + data.cFileName + '/'));

	while (FindNextFileA(handle, &data))
	{
		if (handle != INVALID_HANDLE_VALUE && data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			results.push_back(Directory(_fullpath + data.cFileName + '/'));
	}

	return results;
}

Directory::~Directory()
{
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

std::string Directory::ConvertPath(std::string path)
{
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
