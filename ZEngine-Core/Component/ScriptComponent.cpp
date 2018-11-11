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
		// Create the object defined in the script 
		auto baseInstance = constructor->CallAsConstructor(context, 0, nullptr).ToLocalChecked();

		// Create a template that inherits the script component 
		auto derivedTemplate = v8::FunctionTemplate::New(isolate);
		derivedTemplate->Inherit(scriptSys->GetTemplate(SCRIPT_COMPONENT));
		derivedTemplate->InstanceTemplate()->SetInternalFieldCount(1);
		derivedTemplate->SetClassName(scriptSys->GetString(_script->GetName()));
		
		// Create an instance of the derived template and set the internal field
		auto instance = derivedTemplate->GetFunction()->NewInstance(context).ToLocalChecked();
		instance->SetInternalField(0, v8::External::New(isolate, this));

		// Set the prototype of the derived instance to the base instance
		instance->SetPrototype(context, baseInstance->ToObject(isolate));

		// Keep the derived instance
		_scriptObj.Reset(isolate, instance);
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
	v8::HandleScope handleScope(isolate);
	v8::Context::Scope scope(context);

	// Create the Object that was defined in the script
	auto obj = CreateObjectFromScript(isolate, context);

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
		_scriptInit.Reset(isolate, v8::Local<v8::Function>::Cast(init));

	if (obj->Has(updateString))
		_scriptUpdate.Reset(isolate, v8::Local<v8::Function>::Cast(update));

	if (obj->Has(renderString))
		_scriptRender.Reset(isolate, v8::Local<v8::Function>::Cast(render));

}

void ScriptComponent::InitScript()
{
	if (_scriptInit.IsEmpty()) return;

	auto scriptSys = ScriptSystem::GetInstance();

	// Create a scope
	v8::HandleScope handleScope(scriptSys->GetIsolate());
	v8::Context::Scope scope(scriptSys->GetContext()->GetLocal());

	auto init = _scriptInit.Get(scriptSys->GetIsolate());
	auto obj = _scriptObj.Get(scriptSys->GetIsolate());

	init->Call(obj, 0, nullptr);
}

void ScriptComponent::Update()
{
	if (_scriptUpdate.IsEmpty()) return;

	auto scriptSys = ScriptSystem::GetInstance();

	// Create a scope
	v8::HandleScope handleScope(scriptSys->GetIsolate());
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
	v8::HandleScope handleScope(scriptSys->GetIsolate());
	v8::Context::Scope scope(scriptSys->GetContext()->GetLocal());

	auto render = _scriptRender.Get(scriptSys->GetIsolate());
	auto obj = _scriptObj.Get(scriptSys->GetIsolate());

	render->Call(obj, 0, nullptr);
}

ZObject* ScriptComponent::CreateInstance(std::string name, ObjectType type)
{
	return new ScriptComponent(name);
}

ZObject* ScriptComponent::Copy(std::string name, ZObject* object)
{
	if (object == nullptr || object->GetType() != SCRIPT_COMPONENT)
		return nullptr;

	auto source = static_cast<ScriptComponent*>(object);
	auto copy = new ScriptComponent(name);

	copy->SetScript(source->GetScript());

	return copy;
}

ScriptComponent::~ScriptComponent()
{
	_scriptObj.Reset();
	_scriptInit.Reset();
	_scriptUpdate.Reset();
	_scriptRender.Reset();
}
