#include "ScriptSystem.h"
#include "../Component/Component.h"
#include "../Component/Transform.h"
#include "../Map/Objects/Entity.h"
#include <iostream>

using namespace v8;

ScriptSystem::ScriptSystem()
{
}

// Global callback declarations
void LogCallback(const FunctionCallbackInfo<Value>& info);

bool ScriptSystem::Init(const char* arg0)
{
	// Initialize V8
	V8::InitializeICUDefaultLocation(arg0);
	V8::InitializeExternalStartupData(arg0);

	_platform = platform::NewDefaultPlatform();
	V8::InitializePlatform(_platform.get());

	V8::Initialize();

	// Create a new Isolate
	Isolate::CreateParams create_params;
	_allocator = ArrayBuffer::Allocator::NewDefaultAllocator();
	create_params.array_buffer_allocator = _allocator;

	// Create an isolate (basically the whole JS engine)
	_isolate = Isolate::New(create_params);

	// This will create a persistent handle scope
	_context = new ScriptSystemContext(_isolate);

	// Setup global namespace with custom functions
	auto globalTemplate = ObjectTemplate::New(_isolate);
	SetupCallbacks(globalTemplate);

	// Create a persistent context
	_context->CreateContext(globalTemplate);

	SetupTemplates();

	return _isolate != nullptr;
}

void ScriptSystem::SetupCallbacks(Local<ObjectTemplate>& globalTemplate)
{
	HandleScope handleScope(_isolate);

	globalTemplate->Set(GetString("log"), FunctionTemplate::New(_isolate, LogCallback));
}

void ScriptSystem::SetupTemplates()
{
	Context::Scope scope(_context->GetLocal());

	_templates[ObjectType::SCRIPT_COMPONENT] = Component::GetTemplate(_isolate);
	_templates[ObjectType::ENTITY] = Entity::GetTemplate(_isolate);
	_templates[ObjectType::TRANSFORM] = Transform::GetTemplate(_isolate);

	/*Local<FunctionTemplate> cons = FunctionTemplate::New(_isolate, TestConstructor);
	cons->InstanceTemplate()->SetInternalFieldCount(1);

	global->Set(GetString("TestObject"), cons->GetFunction());

	Local<Object> obj = cons->GetFunction()->NewInstance(_context->GetLocal()).ToLocalChecked();
	obj->SetInternalField(0, External::New(_isolate, this));*/
}

void ScriptSystem::Run(std::string code)
{
	auto context = _context->GetLocal();

	// Enter the context for compiling and running the hello world script.
	v8::Context::Scope context_scope(context);

	{
		// Create a string containing the JavaScript source code.
		v8::Local<v8::String> source = v8::String::NewFromUtf8(_isolate, code.c_str(), v8::NewStringType::kNormal).ToLocalChecked();

		// Compile the source code.
		v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

		// Run the script 
		script->Run(context).ToLocalChecked();
	}
}

v8::Local<v8::String> ScriptSystem::GetString(std::string value) const
{
	return String::NewFromUtf8(_isolate, value.c_str());
}

v8::Local<v8::FunctionTemplate> ScriptSystem::GetTemplate(ObjectType type)
{
	if (_templates.find(type) != _templates.end())
	{
		return _templates[type].Get(_isolate);
	}

	return Local<FunctionTemplate>();
}

bool ScriptSystem::HasTemplate(ObjectType type) const
{
	return _templates.find(type) != _templates.end();
}

Isolate* ScriptSystem::GetIsolate() const
{
	return _isolate;
}

ScriptSystemContext* ScriptSystem::GetContext() const
{
	return _context;
}

void ScriptSystem::Shutdown()
{
	if (_isolate == nullptr) return;

	delete _context;

	_isolate->Dispose();
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();

	delete _allocator;
}

ScriptSystem::~ScriptSystem()
{

}

void LogCallback(const FunctionCallbackInfo<Value>& info)
{
	if (info.Length() <= 0)
	{
		std::cout << "LOG: " << std::endl;
		return;
	}

	auto isolate = info.GetIsolate();
	HandleScope handleScope(isolate);

	auto value = info[0];
	String::Utf8Value stringValue(isolate, value);

	std::cout << "LOG: " << *stringValue << std::endl;
}