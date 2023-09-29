#pragma once

#include "Component.h"

class CSharpScript;

class CSharpScriptComponent : public Component
{
public:
	CSharpScriptComponent(std::string name);
	~CSharpScriptComponent();

	void Init() override;
	void Update() override;
	void Render(int viewId) override;

	void SetScript(CSharpScript* script);
	CSharpScript* GetScript() const;

	void SetManagedInstance(void* instance);
	void* GetManagedInstance() const;

private:
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
