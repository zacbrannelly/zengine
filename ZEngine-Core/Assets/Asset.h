#pragma once

#include "../Map/Objects/ZObject.h"

//TODO: Unused, just using as reference for which types need corresponding asset implementations
enum AssetType
{
	ASSET_TEXTURE, ASSET_MESH, ASSET_FONT, ASSET_ENTITY, ASSET_MATERIAL, ASSET_SHADER, ASSET_MAP, ASSET_SCRIPT
};

enum AssetFileFormat
{
	TEXT, BINARY, CUSTOM
};

class Asset : public ZObject
{
public:
	Asset(std::string name, ObjectType type, AssetFileFormat format);
	virtual ~Asset();

	virtual bool Load(std::string path);

	void* GetData() const;
	unsigned int GetDataSize() const;
	std::string GetDataString() const;
	std::string GetPath() const;

	virtual void Release();
	
	virtual bool IsLoaded() const;

	template<typename T>
	T* Cast()
	{
		return static_cast<T*>(this);
	}

protected:
	void SetData(void* data);
	void SetDataSize(unsigned int size);
	void SetPath(std::string path);
private:
	bool LoadText(std::string& path);
	bool LoadBinary(std::string& path);
	
	std::string _path;
	void* _data;
	unsigned int _size;
	AssetFileFormat _format;
};