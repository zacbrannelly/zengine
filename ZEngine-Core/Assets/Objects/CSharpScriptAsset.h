#pragma once

#include "../Asset.h"

class CSharpScript;

class CSharpScriptAsset : public Asset
{
public:
	CSharpScriptAsset(std::string name);
	~CSharpScriptAsset();

	bool Load(std::string path);
  void Release() override;
	
	CSharpScript* GetScript() const;

	static Asset* CreateInstance(std::string name);

private:
	CSharpScript* _script;
};
