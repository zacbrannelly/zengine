#include "Component.h"
#include "../Scripting/ScriptSystem.h"
#include "../Map/Objects/Entity.h"

using namespace std;

Component::Component(string name, ObjectType type) : ZObject(name, type), IScriptable(type), _owner(nullptr)
{
}

void Component::SetOwner(Entity* owner)
{
	_owner = owner;
}

Entity* Component::GetOwner() const
{
	return _owner;
}

void Callback_Component_GetOwner(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	auto self = info.Holder();
	auto wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));

	auto component = static_cast<Component*>(wrap->Value());

	info.GetReturnValue().Set(component->GetOwner()->GetScriptObject());
}

v8::Global<v8::FunctionTemplate> Component::GetTemplate(v8::Isolate* isolate, v8::Local<v8::Object>& global)
{
	using namespace v8;
	auto sys = ScriptSystem::GetInstance();

	auto constructor = FunctionTemplate::New(isolate);

	constructor->InstanceTemplate()->SetAccessor(String::NewFromUtf8(isolate, "owner"), Callback_Component_GetOwner, nullptr);
	constructor->InstanceTemplate()->SetInternalFieldCount(1);

	// Register the constructor into the global namespace
	global->Set(sys->GetString("Component"), constructor->GetFunction());

	return v8::Global<FunctionTemplate>(isolate, constructor);
}

Component::~Component()
{
}
