#if __APPLE__

#include "Directory.h"
#include "File.h"

#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include <cstdio>
#include <dirent.h>

#define MAX_PATH 260

using namespace std;

bool Directory::Create()
{
  if (!mkdir(_fullpath.c_str(), 0777)) {
    cout << "DIRECTORY: Successfully created the directory '" << _fullpath << "'" << endl;
    return true;
  } else {
    if (errno == EEXIST) {
      cout << "DIRECTORY: Failed to create the directory '" << _fullpath << "' since it already exists!" << endl;
    } else {
      cout << "DIRECTORY: Failed to create the directory '" << _fullpath << "' since the path was invalid!" << endl;
    }
    return false;
  }
}

bool Directory::Move(string newPath)
{
  if (!std::rename(_fullpath.c_str(), newPath.c_str())) {
    cout << "DIRECTORY: Successfully moved the directory '" << _fullpath << "'" << endl;
    _fullpath = newPath;
    return true;
  } else {
    cout << "DIRECTORY: Failed to move the directory '" << _fullpath << "' with error code: " << errno << endl;
    return false;
  }
}

bool Directory::Delete()
{
  if (!std::remove(_fullpath.c_str())) {
    cout << "DIRECTORY: Successfully deleted directory '" << _fullpath << "'" << endl;
    return true;
  } else {
    cout << "DIRECTORY: Failed to delete the directory '" << _fullpath << "' with error code: " << errno << endl;
    return false;
  }
}

bool Directory::Exists() const
{
  struct stat info;
  if (stat(_fullpath.c_str(), &info) != 0) {
    return false;
  } else if (info.st_mode & S_IFDIR) {
    return true;
  }
  return false;
}

bool Directory::IsEmpty() const
{
  return GetAllFiles().empty();
}

string Directory::GetAbsolutePath() const
{
  char buffer[MAX_PATH] = "";
  realpath(_fullpath.c_str(), buffer);
  return ConvertPath(string(buffer));
}

string Directory::GetPathRelativeTo(std::string path) const
{
  string absolutePath = GetAbsolutePath();
  string relativePath = absolutePath.substr(path.length(), absolutePath.length() - path.length());
  return relativePath;
}

bool Directory::FileExists(string filename) const
{
  struct stat buffer;
  const auto result = stat((_fullpath + "/" + filename).c_str(), &buffer);

  if (result == 0 && (buffer.st_mode & S_IFREG)) {
    return true;
  }

  return false;
}

bool Directory::DirectoryExists(string directoryName) const
{
  struct stat buffer;
  const auto result = stat((_fullpath + "/" + directoryName).c_str(), &buffer);

  if (result == 0 && (buffer.st_mode & S_IFDIR)) {
    return true;
  }

  return false;
}

std::vector<File> Directory::GetAllFilesWithExtention(std::string ext) const
{
  const auto files = GetAllFiles();
  std::vector<File> filteredFiles;

  for (const auto& file : files) {
    if (file.GetExtension() == ext) {
      filteredFiles.push_back(file);
    }
  }

  return filteredFiles;
}

std::vector<File> Directory::GetAllFiles() const
{
	vector<File> results;

  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(_fullpath.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_type == DT_REG) {
        string filename = ent->d_name;
        results.push_back(File(_fullpath + "/" + filename));
      }
    }
    closedir(dir);
  } else {
    cout << "DIRECTORY: Failed to open directory '" << _fullpath << "' with error code: " << errno << endl;
    throw std::runtime_error("Failed to open directory '" + _fullpath + "'");
  }

  return results;
}

std::vector<Directory> Directory::GetAllDirectories() const
{
  vector<Directory> results;

  DIR *dir;
  struct dirent *ent;

  if ((dir = opendir(_fullpath.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_type == DT_DIR) {
        string filename = ent->d_name;
        results.push_back(Directory(_fullpath + "/" + filename));
      }
    }
    closedir(dir);
  } else {
    cout << "DIRECTORY: Failed to open directory '" << _fullpath << "' with error code: " << errno << endl;
    throw std::runtime_error("Failed to open directory '" + _fullpath + "'");
  }

  return results;
}

#endif
