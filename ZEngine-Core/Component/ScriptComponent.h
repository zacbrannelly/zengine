#pragma once

#include "Component.h"
#include <include/v8.h>

class Script;

class ScriptComponent : public Component
{
public:
	ScriptComponent(std::string name);
	~ScriptComponent();

	void Init() override;
	void Update() override;
	void Render(int viewId) override;

	void SetScript(Script* script);
	Script* GetScript() const;

private:
	v8::Persistent<v8::Function> _scriptConstructor, _scriptInit, _scriptUpdate, _scriptRender;
	v8::Persistent<v8::Object> _scriptObj;
	Script* _script;
public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
};

