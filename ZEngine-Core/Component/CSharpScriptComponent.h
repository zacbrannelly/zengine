#pragma once

#include "Component.h"
#include "../Utilities/JsonHelpers.h"
#include "../Assets/Objects/CSharpScriptAsset.h"
#include <uuid.h>

namespace ZEngine
{
	class CSharpScriptComponent : public Component
	{
	public:
		CSharpScriptComponent();
		~CSharpScriptComponent();

		void Init() override;
		void Update() override;
		void Render(int viewId) override;

		void SetScript(CSharpScript* script);
		CSharpScript* GetScript() const;

		void SetManagedInstance(void* instance);
		void* GetManagedInstance() const;

	#ifndef SWIG
		// Allow serialization / deserialization
		JSON_SCHEMA_BEGIN(CSharpScriptComponent)
			INCLUDE_ASSET_REFERENCES()
			JSON_MAP_TO_ASSET_REF_SETTER(script, SetScriptFromAsset, CSharpScriptAsset)
		JSON_SCHEMA_END()
	#endif

	private:
		void SetScriptFromAsset(CSharpScriptAsset* scriptAsset) { SetScript(scriptAsset->GetScript()); }
		CSharpScript* _script;
		void* _managedInstance;

	public:
		static ZObject* CreateInstance(std::string name, ObjectType type);
		static ZObject* Copy(std::string name, ZObject* object);

		static ObjectType GetStaticType()
		{
			return CSHARP_SCRIPT_COMPONENT;
		}
	};
}
