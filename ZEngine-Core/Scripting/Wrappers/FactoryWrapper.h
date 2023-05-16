#pragma once

#include "Wrapper.h"

class FactoryWrapper : public Wrapper<FactoryWrapper>
{
public:
	FactoryWrapper* ConstructorImpl(const v8::FunctionCallbackInfo<v8::Value>& info) override;
	void InstallImpl(v8::Local<v8::ObjectTemplate> temp) override;
	void GetterImpl(std::string name, const v8::PropertyCallbackInfo<v8::Value>& info) override;
	void SetterImpl(std::string name, v8::Local<v8::Value>& value) override;

	static void RegisterEnums();
};

