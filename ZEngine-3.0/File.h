#pragma once

#include <string>

class File
{
public:
	File(std::string path);
	~File();

	virtual bool Create();
	virtual bool Move(std::string newPath);
	virtual bool Delete();

	virtual bool Exists() const;

	virtual bool ReadContent(char* buffer, int bufferLen) const;
	virtual bool WriteContent(const char* buffer, int bufferLen) const;

	const std::string& GetFilename() const;
	const std::string& GetPath() const;

private:
	std::string _path;
	std::string _filename;
};