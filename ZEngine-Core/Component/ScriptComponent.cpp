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

v8::Local<v8::Object> ScriptComponent::CreateObjectFromScript(v8::Isolate*& isolate, v8::Local<v8::Context>& context)
{
	auto scriptSys = ScriptSystem::GetInstance();

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
		return v8::Local<v8::Object>();
	}

	auto constructor = _scriptConstructor.Get(isolate);

	if (constructor->IsConstructor())
	{
		// Construct the object defined in the script
		_scriptObj.Reset(isolate, constructor->NewInstance(context).ToLocalChecked());
	}
	else
	{
		std::cout << "SCRIPT_COMPONENT: Script class name is not a constructor '" << _script->GetName() << "'" << std::endl;
		return v8::Local<v8::Object>();
	}

	return _scriptObj.Get(isolate);
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

	// Create the Object that was defined in the script
	auto obj = CreateObjectFromScript(isolate, context);

	// Set the internal field for the parent (which should be defined as the Component function template)
	auto componentObj = obj->FindInstanceInPrototypeChain(scriptSys->GetTemplate(ObjectType::SCRIPT_COMPONENT));
	componentObj->SetInternalField(0, v8::External::New(isolate, this));

	// Remove our previously created script object and set to the new one
	SetScriptObject(obj);

	// Retrieve the functions from the object (TODO: make this more automated)
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
	_scriptObj.Reset();
	_scriptInit.Reset();
	_scriptUpdate.Reset();
	_scriptRender.Reset();
}
