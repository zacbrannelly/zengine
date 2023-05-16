#include "TimeWrapper.h"
#include "../../Physics/Time.h"

TimeWrapper* TimeWrapper::ConstructorImpl(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	return GetInstance();
}

void TimeWrapper::InstallImpl(v8::Local<v8::ObjectTemplate> temp)
{
	auto sys = ScriptSystem::GetInstance();

	temp->SetAccessor(sys->GetString("delta"), Getter, Setter);
	temp->SetAccessor(sys->GetString("time"), Getter, Setter);
	temp->SetAccessor(sys->GetString("fps"), Getter, Setter);
}

void TimeWrapper::GetterImpl(std::string name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	auto time = Time::GetInstance();

	if (name == "delta")
	{
		info.GetReturnValue().Set(time->GetDeltaTime());
	}
	else if (name == "time")
	{
		info.GetReturnValue().Set(time->GetTime());
	}
	else if (name == "fps")
	{
		info.GetReturnValue().Set(time->GetFPS());
	}
}

void TimeWrapper::SetterImpl(std::string name, v8::Local<v8::Value>& value)
{

}