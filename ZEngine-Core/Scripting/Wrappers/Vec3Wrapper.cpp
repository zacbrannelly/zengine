#include "Vec3Wrapper.h"
#include "../ScriptSystem.h"

using namespace v8;

Vec3Wrapper* Vec3Wrapper::ConstructorImpl(const FunctionCallbackInfo<Value>& info)
{
	auto newInstance = new Vec3Wrapper();
	auto sys = ScriptSystem::GetInstance();
	auto context = sys->GetContext()->GetLocal();

	if (info.Length() >= 3 && info[0]->IsNumber() && info[1]->IsNumber() && info[2]->IsNumber())
	{
		newInstance->_value.x = info[0]->NumberValue(context).ToChecked();
		newInstance->_value.y = info[1]->NumberValue(context).ToChecked();
		newInstance->_value.z = info[2]->NumberValue(context).ToChecked();
	}
	else if (info.Length() == 2 && info[0]->IsNumber() && info[1]->IsNumber())
	{
		newInstance->_value.x = info[0]->NumberValue(context).ToChecked();
		newInstance->_value.y = info[1]->NumberValue(context).ToChecked();
	}
	else if (info.Length() == 1 && info[0]->IsNumber())
	{
		newInstance->_value.x = info[0]->NumberValue(context).ToChecked();
		newInstance->_value.y = newInstance->_value.x;
		newInstance->_value.z = newInstance->_value.x;
	}

	return newInstance;
}

void Vec3Wrapper::InstallImpl(v8::Local<v8::ObjectTemplate>& temp)
{
	auto sys = ScriptSystem::GetInstance();

	temp->SetAccessor(sys->GetString("x"), Getter, Setter);
	temp->SetAccessor(sys->GetString("y"), Getter, Setter);
	temp->SetAccessor(sys->GetString("z"), Getter, Setter);
}

void Vec3Wrapper::GetterImpl(std::string name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	if (name == "x")
	{
		info.GetReturnValue().Set(_value.x);
	}
	else if (name == "y")
	{
		info.GetReturnValue().Set(_value.y);
	}
	else if (name == "z")
	{
		info.GetReturnValue().Set(_value.z);
	}
}

void Vec3Wrapper::SetterImpl(std::string name, v8::Local<v8::Value>& value)
{
	auto context = ScriptSystem::GetInstance()->GetContext()->GetLocal();

	if (!value->IsNumber()) return;

	if (name == "x")
	{
		_value.x = value->NumberValue(context).ToChecked();
	}
	else if (name == "y")
	{
		_value.y = value->NumberValue(context).ToChecked();
	}
	else if (name == "z")
	{
		_value.z = value->NumberValue(context).ToChecked();
	}
}

void Vec3Wrapper::SetData(glm::vec3 value)
{
	_value = value;
}

glm::vec3 Vec3Wrapper::GetData() const
{
	return _value;
}
