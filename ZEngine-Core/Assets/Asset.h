#pragma once

#include "../Map/Objects/ZObject.h"

class Asset : public ZObject
{
public:
	Asset(std::string name, ObjectType type);
	virtual ~Asset();

	virtual bool Load(std::string path) = 0;
	virtual void Release();

	std::string GetPath() const;

	bool IsLoaded() const;

	template<typename T> T* Cast()
	{
		return static_cast<T*>(this);
	}

	static ObjectType GetStaticType()
	{
		return ASSET;
	}

protected:
	void SetPath(std::string path);
	void SetLoaded(bool loaded);

	bool LoadText(const std::string& path, std::string& result);
	bool LoadBinary(const std::string& path, char* result, unsigned int& size);

private:
	std::string _path;
	bool _isLoaded;
};