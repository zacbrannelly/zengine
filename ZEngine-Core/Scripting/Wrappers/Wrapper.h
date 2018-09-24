#pragma once

#include <include/v8.h>
#include "../ScriptSystem.h"
#include "../../Misc/OneShotSingleton.h"
#include <iostream>

template<typename T>
class Wrapper : public OneShotSingleton<T>
{
public:

	// Abstract methods to be overidden by the child classes (not actually abstract but they must be overidden!)
	virtual T* ConstructorImpl(const v8::FunctionCallbackInfo<v8::Value>& info) { return nullptr; }
	virtual void InstallImpl(v8::Local<v8::ObjectTemplate>& temp) {}
	virtual void GetterImpl(std::string name, const v8::PropertyCallbackInfo<v8::Value>& info) { }
	virtual void SetterImpl(std::string name, v8::Local<v8::Value>& value) {}

	v8::Local<v8::Object> GetObject() const
	{
		return _instanceObj.Get(ScriptSystem::GetInstance()->GetIsolate());
	}
 
	static void Install(std::string name, v8::Local<v8::Object>& global);
	static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info);
	static void Destructor(const v8::WeakCallbackInfo<T>& info);
	static void DestroyTemplate();

	static T* NewInstance();

	static void Getter(v8::Local<v8::String> nameObj, const v8::PropertyCallbackInfo<v8::Value>& info);
	static void Setter(v8::Local<v8::String> nameObj, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

private:
	v8::Persistent<v8::Object> _instanceObj;
	static v8::Global<v8::FunctionTemplate> _template;
};


template<typename T>
v8::Global<v8::FunctionTemplate> Wrapper<T>::_template;

template<typename T>
T* Wrapper<T>::NewInstance()
{
	auto sys = ScriptSystem::GetInstance();
	auto context = sys->GetContext()->GetLocal();

	v8::Context::Scope scope(context);

	// Call the constructor through the JS engine
	auto newObj = _template.Get(sys->GetIsolate())->GetFunction()->NewInstance(context).ToLocalChecked();

	// Return the pointer set when the new obj was created
	return static_cast<T*>(v8::Local<v8::External>::Cast(newObj->GetInternalField(0))->Value());
}

template<typename T>
void Wrapper<T>::Install(std::string name, v8::Local<v8::Object>& global)
{
	using namespace v8;

	// Get the ScriptSystem and the global wrapper instance
	auto sys = ScriptSystem::GetInstance();
	auto instance = GetInstance();
	
	// Create a template with a constructor
	auto temp = FunctionTemplate::New(sys->GetIsolate(), Constructor);
	temp->InstanceTemplate()->SetInternalFieldCount(1);

	// Install custom properties and functions
	instance->InstallImpl(temp->InstanceTemplate());

	// Install the constructor in the global namespace
	global->Set(sys->GetString(name), temp->GetFunction());

	// Save the function template
	_template.Reset(sys->GetIsolate(), temp);
}

template<typename T>
void Wrapper<T>::Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto sys = ScriptSystem::GetInstance();
	auto instance = Wrapper<T>::GetInstance();

	// Create new instance of the wrapper and set to the object
	auto newInstance = instance->ConstructorImpl(info);

	// Get the new javascript object
	auto jsObject = info.Holder();

	// Set a weak reference to the object so we know when it is collected
	newInstance->_instanceObj.Reset(info.GetIsolate(), jsObject);
	newInstance->_instanceObj.SetWeak<T>(newInstance, Destructor, v8::WeakCallbackType::kParameter);
	
	// Set the new C++ instance to the internal field of the js object
	jsObject->SetInternalField(0, External::New(info.GetIsolate(), newInstance));
}

template<typename T>
void Wrapper<T>::Destructor(const v8::WeakCallbackInfo<T>& info)
{
	if (info.GetParameter() != nullptr)
	{
		auto instance = info.GetParameter();

		// Reset the weak handle
		instance->_instanceObj.Reset();

		// Delete the instance from the heap (avoid memory leaks)
		delete instance;
	}
}

template<typename T>
void Wrapper<T>::DestroyTemplate()
{
	_template.Reset();
}

template<typename T>
void Wrapper<T>::Setter(v8::Local<v8::String> nameObj, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	auto sys = ScriptSystem::GetInstance();
	auto name = sys->CastString(nameObj);

	using namespace v8;

	// Get the instance of the object that called this setter
	T* instance = static_cast<T*>(Local<External>::Cast(info.Holder()->GetInternalField(0))->Value());

	if (instance != nullptr)
	{
		instance->SetterImpl(name, value);
	}
}

template<typename T>
void Wrapper<T>::Getter(v8::Local<v8::String> nameObj, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	auto sys = ScriptSystem::GetInstance();
	auto name = sys->CastString(nameObj);

	using namespace v8;

	// Get the instance of the object that called this getter
	T* instance = static_cast<T*>(Local<External>::Cast(info.Holder()->GetInternalField(0))->Value());

	if (instance != nullptr)
	{
		instance->GetterImpl(name, info);
	}
}