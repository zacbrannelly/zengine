#include "IScriptable.h"
#include "ScriptSystem.h"
#include <iostream>

IScriptable::IScriptable(ObjectType type)
{
	if (ScriptSystem::GetInstance()->HasTemplate(type))
	{
		auto isolate = ScriptSystem::GetInstance()->GetIsolate();
		auto context = ScriptSystem::GetInstance()->GetContext()->GetLocal();
		v8::Context::Scope scope(context);

		// Get the template for this object from the script system
		auto objTemplate = ScriptSystem::GetInstance()->GetTemplate(type);

		// Create the JS object for this instance and link to the native pointer
		auto newObj = objTemplate->GetFunction()->NewInstance(context);
		newObj.ToLocalChecked()->SetInternalField(0, v8::External::New(isolate, this));

		// Put it into the persistent data
		_scriptInstance.Reset(isolate, newObj.ToLocalChecked());
	}
	else
		std::cout << "ISCRIPTABLE: No template found for object with type: " << type << std::endl;
}

v8::Local<v8::Object> IScriptable::GetScriptObject() const
{
	return !_scriptInstance.IsEmpty() ? _scriptInstance.Get(ScriptSystem::GetInstance()->GetIsolate()) : v8::Local<v8::Object>();
}

IScriptable::~IScriptable()
{
	_scriptInstance.Reset();
}
