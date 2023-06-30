#pragma once

#include <string>
#include <nlohmann/json.hpp>

class File
{
public:
	File(std::string path);
	~File();

	virtual void Set(std::string path);

	virtual bool Create();
	virtual bool Move(std::string newPath);
	virtual bool Copy(std::string newPath);
	virtual bool Delete();

	virtual bool Exists() const;

	virtual bool ReadContent(char* buffer, int bufferLen, bool isBinary = false) const;
	virtual bool WriteContent(const char* buffer, int bufferLen, bool isBinary = false) const;

	virtual nlohmann::json ReadJson() const;
	virtual bool WriteJson(const nlohmann::json& json) const;

	const std::string& GetFilename() const;
	const std::string& GetPath() const;
	std::string GetRelativePath(std::string to = "./") const;
	std::string GetName() const;
	std::string GetExtension() const;
	std::string GetDirectory() const;
private:
	std::string _path;
	std::string _filename;
};
