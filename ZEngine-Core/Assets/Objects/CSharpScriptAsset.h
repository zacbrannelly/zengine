#pragma once

#include "../Asset.h"
#include "../../Scripting/CSharp/CSharpScript.h"
#include "../../Utilities/JsonHelpers.h"

namespace ZEngine 
{
	class CSharpScriptAsset : public Asset
	{
	public:
		CSharpScriptAsset(std::string name);
		~CSharpScriptAsset();

		bool Load(std::string path);
		void Release() override;
		
		CSharpScript* GetScript() const;
		std::string GetScriptPath() const;

		static ZObject* CreateInstance(std::string name, ObjectType type);
		
		static ObjectType GetStaticType()
		{
			return CSHARP_SCRIPT_ASSET;
		}
	private:
		CSharpScript* _script;
	};
}
