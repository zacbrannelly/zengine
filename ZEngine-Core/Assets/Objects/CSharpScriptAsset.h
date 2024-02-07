#pragma once

#include "../Asset.h"
#include "../../Scripting/CSharp/CSharpScript.h"
#include "../../Utilities/JsonHelpers.h"

class CSharpScriptAsset : public Asset
{
public:
	CSharpScriptAsset(std::string name);
	~CSharpScriptAsset();

	bool Load(std::string path);
  void Release() override;
	
	CSharpScript* GetScript() const;
	std::string GetScriptPath() const;

	static Asset* CreateInstance(std::string name);
	
	static ObjectType GetStaticType()
	{
		return CSHARP_SCRIPT_ASSET;
	}
private:
	CSharpScript* _script;
};
