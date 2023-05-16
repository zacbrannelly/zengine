#include "FactoryWrapper.h"
#include "../../Misc/Factory.h"
#include "../../Component/Component.h"
#include "../../Map/Objects/Entity.h"

using namespace std;

#define REGISTER(x) global->Set(context, system->GetString(#x), v8::Int32::New(system->GetIsolate(), x))

FactoryWrapper* FactoryWrapper::ConstructorImpl(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	return GetInstance();
}

// TODO: Figure out a better way to handle this
bool IsComponent(ObjectType type)
{
	return type == TRANSFORM || type == CAMERA || type == MESH_RENDERER || type == SCRIPT_COMPONENT;
}

void Callback_Factory_CreateInstance(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsInt32())
		return;

	auto sys = ScriptSystem::GetInstance();
	auto context = sys->GetContext()->GetLocal();

	auto name = sys->CastString(info[0]->ToString(context).ToLocalChecked());
	auto type = (ObjectType)info[1]->ToInt32(context).ToLocalChecked()->Int32Value(context).ToChecked();

	if (sys->HasTemplate(type))
	{
		auto instance = Factory::CreateInstance<ZObject>(name, type);
		info.GetReturnValue().Set(instance->GetScriptObject());
	}
	else
		info.GetReturnValue().SetNull();
}

void Callback_Factory_Copy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsObject())
		return;

	auto sys = ScriptSystem::GetInstance();
	auto context = sys->GetContext()->GetLocal();
	
	auto name = sys->CastString(info[0]->ToString(context).ToLocalChecked());
	auto wrap = v8::Local<v8::External>::Cast(info[1]->ToObject(context).ToLocalChecked()->GetInternalField(0));
	auto scriptable = static_cast<IScriptable*>(wrap->Value());
	auto object = static_cast<ZObject*>(scriptable);

	auto copy = Factory::Copy<ZObject>(name, object);

	if (copy != nullptr)
	{
		info.GetReturnValue().Set(copy->GetScriptObject());
	}
	else
		info.GetReturnValue().SetNull();
}

void FactoryWrapper::InstallImpl(v8::Local<v8::ObjectTemplate> temp)
{
	auto sys = ScriptSystem::GetInstance();
	auto isolate = sys->GetIsolate();

	temp->Set(isolate, "Create", v8::FunctionTemplate::New(isolate, Callback_Factory_CreateInstance));
	temp->Set(isolate, "Copy", v8::FunctionTemplate::New(isolate, Callback_Factory_Copy));
}

void FactoryWrapper::RegisterEnums()
{
	auto system = ScriptSystem::GetInstance();
	auto context = system->GetContext()->GetLocal();
	auto global = context->Global();

	REGISTER(SCRIPT_COMPONENT);
	REGISTER(MESH_RENDERER);
	REGISTER(TRANSFORM);
	REGISTER(CAMERA);

	REGISTER(MAP_ASSET);
	REGISTER(MATERIAL_ASSET);
	REGISTER(MODEL_ASSET);
	REGISTER(SCRIPT_ASSET);
	REGISTER(SHADER_ASSET);
	REGISTER(TEXTURE_ASSET);

	REGISTER(MAP);
	REGISTER(MESH);
	REGISTER(MATERIAL);
	REGISTER(SHADER);
	REGISTER(ENTITY);
}

void FactoryWrapper::GetterImpl(string name, const v8::PropertyCallbackInfo<v8::Value>& info)
{

}

void FactoryWrapper::SetterImpl(string name, v8::Local<v8::Value>& value)
{

}
