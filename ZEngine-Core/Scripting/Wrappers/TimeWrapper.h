#pragma once

#include <v8.h>
#include <glm\glm.hpp>
#include "Wrapper.h"

class Time;

class TimeWrapper : public Wrapper<TimeWrapper>
{
public:
	TimeWrapper* ConstructorImpl(const v8::FunctionCallbackInfo<v8::Value>& info) override;
	void InstallImpl(v8::Local<v8::ObjectTemplate>& temp) override;
	void GetterImpl(std::string name, const v8::PropertyCallbackInfo<v8::Value>& info) override;
	void SetterImpl(std::string name, v8::Local<v8::Value>& value) override;
};
