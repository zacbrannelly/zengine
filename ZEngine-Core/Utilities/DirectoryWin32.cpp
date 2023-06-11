#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

#include "Directory.h"
#include "File.h"

#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <Shlwapi.h>

using namespace std;

bool Directory::Create()
{
	if (CreateDirectoryA(_fullpath.c_str(), nullptr))
	{
		cout << "DIRECTORY: Successfully created the directory '" << _fullpath << "'" << endl;
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
		cout << "DIRECTORY: Successfully moved the directory '" << _fullpath << "'" << endl;
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

#endif
