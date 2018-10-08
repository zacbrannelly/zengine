#pragma once

#include "../Asset.h"

class Script;

class ScriptAsset : public Asset
{
public:
	ScriptAsset(std::string name);
	~ScriptAsset();

	bool Load(std::string path);
	
	Script* GetScript() const;

	static Asset* CreateInstance(std::string name);

private:
	Script* _script;
};

