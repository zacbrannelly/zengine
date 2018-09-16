#pragma once

#include "../Misc/Singleton.h"
#include "../Map/Objects/ZObject.h"
#include <map>
#include <include/v8.h>
#include <include/libplatform/libplatform.h>

struct ScriptSystemContext
{
	ScriptSystemContext(v8::Isolate* _isolate) : handleScope(_isolate), isolate(_isolate), isolateScope(_isolate)
	{
		auto localContext = v8::Context::New(isolate);
		context.Reset(isolate, localContext);
	}

	void CreateContext(v8::Local<v8::ObjectTemplate>& globalTemplate)
	{
		auto localContext = v8::Context::New(isolate, nullptr, globalTemplate);
		context.Reset(isolate, localContext);
	}

	v8::Local<v8::Context> GetLocal()
	{
		return context.Get(isolate);
	}

	v8::Isolate* isolate;
	v8::HandleScope handleScope;
	v8::Isolate::Scope isolateScope;
	v8::Global<v8::Context> context;
};

class ScriptSystem : public Singleton<ScriptSystem>
{
public:
	ScriptSystem();
	~ScriptSystem();

	bool Init(const char* arg0);
	void Shutdown();

	v8::Isolate* GetIsolate() const;
	ScriptSystemContext* GetContext() const;

	void Run(std::string code);
	v8::Local<v8::String> GetString(std::string value) const;

	v8::Local<v8::FunctionTemplate> GetTemplate(ObjectType type);
	bool HasTemplate(ObjectType type) const;

private:
	void SetupCallbacks(v8::Local<v8::ObjectTemplate>& globalTemplate);
	void SetupTemplates();

	v8::Isolate* _isolate;
	std::unique_ptr<v8::Platform> _platform;
	v8::ArrayBuffer::Allocator* _allocator;
	ScriptSystemContext* _context;

	std::map<ObjectType, v8::Global<v8::FunctionTemplate>> _templates;
};

