#include "ScriptComponent.h"
#include "../Scripting/Script.h"
#include "../Scripting/ScriptSystem.h"
#include "../Map/Objects/Entity.h"
#include <iostream>

ScriptComponent::ScriptComponent(std::string name) : Component(name, ObjectType::SCRIPT_COMPONENT)
{
	_script = nullptr;
}

void ScriptComponent::SetScript(Script* script)
{
	_script = script;
}

Script* ScriptComponent::GetScript() const
{
	return _script;
}

void ScriptComponent::Init()
{
	if (_script == nullptr)
	{
		std::cout << "SCRIPT_COMPONENT: Failed to initialize since no script was set for '" << GetName() << "' !" << std::endl;
		return;
	}

	auto scriptSys = ScriptSystem::GetInstance();
	auto isolate = scriptSys->GetIsolate();
	auto context = scriptSys->GetContext()->GetLocal();

	// Create a scope
	v8::Context::Scope scope(context);

	// Get the global namespace
	auto global = context->Global();

	// Try to get the class constructor from the global namespace
	auto value = global->Get(scriptSys->GetString(_script->GetName()));

	if (!value.IsEmpty() && value->IsFunction())
	{
		_scriptConstructor.Reset(isolate, v8::Local<v8::Function>::Cast(value));
	}
	else
	{
		std::cout << "SCRIPT_COMPONENT: Failed to find the constructor for script '" << _script->GetName() << "'" << std::endl;
		return;
	}

	auto constructor = _scriptConstructor.Get(isolate);

	if (constructor->IsConstructor())
	{
		_scriptObj.Reset(isolate, constructor->NewInstance(context).ToLocalChecked());
	}
	else
	{
		std::cout << "SCRIPT_COMPONENT: Script class name is not a constructor '" << _script->GetName() << "'" << std::endl;
		return;
	}

	auto obj = _scriptObj.Get(isolate);

	// TODO: Add extra functions and fields to the object 
	obj->Set(scriptSys->GetString("entity"), GetOwner()->GetScriptObject());

	auto initString = scriptSys->GetString("Init");
	auto updateString = scriptSys->GetString("Update");
	auto renderString = scriptSys->GetString("Render");

	auto init = obj->Get(initString);
	auto update = obj->Get(updateString);
	auto render = obj->Get(renderString);

	if (obj->Has(initString))
	{
		_scriptInit.Reset(isolate, v8::Local<v8::Function>::Cast(init));

		// Call Init function in the script
		v8::Local<v8::Function>::Cast(init)->Call(obj, 0, nullptr);
	}

	if (obj->Has(updateString))
		_scriptUpdate.Reset(isolate, v8::Local<v8::Function>::Cast(update));

	if (obj->Has(renderString))
		_scriptRender.Reset(isolate, v8::Local<v8::Function>::Cast(render));

}

void ScriptComponent::Update()
{
	if (_scriptUpdate.IsEmpty()) return;

	auto scriptSys = ScriptSystem::GetInstance();

	// Create a scope
	v8::Context::Scope scope(scriptSys->GetContext()->GetLocal());

	auto update = _scriptUpdate.Get(scriptSys->GetIsolate());
	auto obj = _scriptObj.Get(scriptSys->GetIsolate());

	update->Call(obj, 0, nullptr);
}

void ScriptComponent::Render(int viewId)
{
	if (_scriptRender.IsEmpty()) return;

	auto scriptSys = ScriptSystem::GetInstance();

	// Create a scope
	v8::Context::Scope scope(scriptSys->GetContext()->GetLocal());

	auto render = _scriptRender.Get(scriptSys->GetIsolate());
	auto obj = _scriptObj.Get(scriptSys->GetIsolate());

	render->Call(obj, 0, nullptr);
}

ZObject * ScriptComponent::CreateInstance(std::string name, ObjectType type)
{
	return new ScriptComponent(name);
}

ScriptComponent::~ScriptComponent()
{
}
