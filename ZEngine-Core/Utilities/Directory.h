#pragma once

#include <string>
#include <vector>

namespace ZEngine
{
	class File;

	class Directory
	{
	public:
		Directory(std::string path);
		virtual ~Directory();

		virtual void Set(std::string newPath);

		virtual bool Create();
		virtual bool Move(std::string newPath);
		virtual bool Delete();

		virtual bool Exists() const;
		virtual bool IsEmpty() const;

		const std::string& GetName() const;
		const std::string& GetPath() const;
		std::string GetAbsolutePath() const;
		std::string GetPathRelativeTo(std::string path = "./") const;

		Directory GetParentDirectory() const;

		virtual File FindFile(std::string filename) const;
		virtual Directory FindDirectory(std::string directoryName) const;

		virtual bool FileExists(std::string filename) const;
		virtual bool DirectoryExists(std::string directoryName) const;

		virtual std::vector<File> GetAllFilesWithExtention(std::string ext) const;
		virtual std::vector<File> GetAllFiles() const;
		virtual std::vector<Directory> GetAllDirectories() const;

	private:
		std::string AddTrailingSlash(std::string path) const;

		std::string _fullpath;
		std::string _name;

	public:
		static std::string GetFilename(std::string fullpath);
		static std::string GetExtension(std::string fullpath);
		static std::string GetBasePath(std::string fullpath);
		static char GetPathDelimiter(std::string path);
		static std::string ConvertPathDelimiter(std::string path, char delim);
		static std::string ConvertPath(std::string path);
		static std::string GetCurrentWorkingDirectory();
		static std::string GetAbsolutePath(std::string path);
	};
}
