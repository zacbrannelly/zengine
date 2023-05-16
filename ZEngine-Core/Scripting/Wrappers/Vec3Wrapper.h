#pragma once

#include "Wrapper.h"
#include "glm/glm.hpp"

class Vec3Wrapper : public Wrapper<Vec3Wrapper>
{
public:
	
	Vec3Wrapper* ConstructorImpl(const v8::FunctionCallbackInfo<v8::Value>& info) override;
	void InstallImpl(v8::Local<v8::ObjectTemplate> temp) override;
	void GetterImpl(std::string name, const v8::PropertyCallbackInfo<v8::Value>& info) override;
	void SetterImpl(std::string name, v8::Local<v8::Value>& value) override;

	void SetData(glm::vec3 value);
	glm::vec3 GetData() const;

private:
	glm::vec3 _value;

};

