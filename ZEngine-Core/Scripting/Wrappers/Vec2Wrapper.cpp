#include "Vec2Wrapper.h"
#include "../ScriptSystem.h"

using namespace v8;

Vec2Wrapper* Vec2Wrapper::ConstructorImpl(const FunctionCallbackInfo<Value>& info)
{
	auto newInstance = new Vec2Wrapper();
	auto sys = ScriptSystem::GetInstance();
	auto context = sys->GetContext()->GetLocal();

	if (info.Length() >= 2 && info[0]->IsNumber() && info[1]->IsNumber())
	{
		newInstance->_value.x = info[0]->NumberValue(context).ToChecked();
		newInstance->_value.y = info[1]->NumberValue(context).ToChecked();
	}
	else if (info.Length() == 1 && info[0]->IsNumber())
	{
		newInstance->_value.x = info[0]->NumberValue(context).ToChecked();
		newInstance->_value.y = newInstance->_value.x;
	}

	return newInstance;
}

void Vec2Wrapper::InstallImpl(v8::Local<v8::ObjectTemplate>& temp)
{
	auto sys = ScriptSystem::GetInstance();

	temp->SetAccessor(sys->GetString("x"), Getter, Setter);
	temp->SetAccessor(sys->GetString("y"), Getter, Setter);
}

void Vec2Wrapper::GetterImpl(std::string name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	if (name == "x")
	{
		info.GetReturnValue().Set(_value.x);
	}
	else if (name == "y")
	{
		info.GetReturnValue().Set(_value.y);
	}
}

void Vec2Wrapper::SetterImpl(std::string name, v8::Local<v8::Value>& value)
{
	auto context = ScriptSystem::GetInstance()->GetContext()->GetLocal();

	if (name == "x" && value->IsNumber())
	{
		_value.x = value->NumberValue(context).ToChecked();
	}
	else if (name == "y")
	{
		_value.y = value->NumberValue(context).ToChecked();
	}
}

void Vec2Wrapper::SetData(glm::vec2 vec)
{
	_value = vec;
}

glm::vec2 Vec2Wrapper::GetData() const
{
	return _value;
}