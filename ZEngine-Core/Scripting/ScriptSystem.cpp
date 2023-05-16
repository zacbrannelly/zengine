#include "ScriptSystem.h"
#include "../Component/Component.h"
#include "../Component/Transform.h"
#include "../Component/AudioSource.h"
#include "../Map/Objects/Entity.h"
#include "../Input/InputManager.h"
#include "../Map/Map.h"

#include "../Scripting/Wrappers/Vec2Wrapper.h"
#include "../Scripting/Wrappers/Vec3Wrapper.h"
#include "../Scripting/Wrappers/TimeWrapper.h"
#include "../Scripting/Wrappers/MapManagerWrapper.h"
#include "../Scripting/Wrappers/FactoryWrapper.h"

#include <iostream>

// Global callback declarations
void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

bool ScriptSystem::Init(const char* arg0)
{
	using namespace v8;

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
	SetupGlobalBindings();

	return _isolate != nullptr;
}

void ScriptSystem::SetupCallbacks(v8::Local<v8::ObjectTemplate>& globalTemplate)
{
	v8::HandleScope handleScope(_isolate);

	globalTemplate->Set(GetString("log"), v8::FunctionTemplate::New(_isolate, LogCallback));
}

void ScriptSystem::SetupTemplates()
{
	v8::Context::Scope scope(_context->GetLocal());
	auto global = _context->GetLocal()->Global();

	// Setup template bindings for objects that are used inside JS that are not global
	// These objects have no C++ constructors
	_templates[ObjectType::SCRIPT_COMPONENT] = Component::GetTemplate(_isolate, global);
	_templates[ObjectType::ENTITY] = Entity::GetTemplate(_isolate);
	_templates[ObjectType::TRANSFORM] = Transform::GetTemplate(_isolate);
	_templates[ObjectType::MAP] = Map::GetTemplate(_isolate);
	_templates[ObjectType::AUDIO_SOURCE] = AudioSource::GetTemplate(_isolate);

	// Setup basic classes that can be created in JS code (have C++ constructors)
	Vec2Wrapper::Install("vec2", global);
	Vec3Wrapper::Install("vec3", global);
	MapManagerWrapper::Install("MAP_MANAGER", global);
	TimeWrapper::Install("TIME_OBJECT", global);
	FactoryWrapper::Install("FACTORY_OBJECT", global);
}

void ScriptSystem::SetupGlobalBindings()
{
	auto localContext = _context->GetLocal();
	v8::Context::Scope scope(localContext);
	auto global = localContext->Global();

	// TODO: Setup global bindings (objects that are attached to the global namespace)

	// Bind the Input Manager functions
	InputManager::GetInstance()->SetupScriptBindings(_isolate, global);

	// Bind the Input Button Codes
	InstallButtonCodesIntoScripting(this); // From InputTypes.cpp

	FactoryWrapper::RegisterEnums();

	// Bind the global objects (managers usually) 
	global->Set(localContext, GetString("Time"), TimeWrapper::NewInstance()->GetObject());
	global->Set(localContext, GetString("MapManager"), MapManagerWrapper::NewInstance()->GetObject());
	global->Set(localContext, GetString("Factory"), FactoryWrapper::NewInstance()->GetObject());
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
	return v8::String::NewFromUtf8(_isolate, value.c_str()).ToLocalChecked();
}

std::string ScriptSystem::CastString(v8::Local<v8::String> stringObj) const
{
	v8::String::Utf8Value utfString(_isolate, stringObj);
	return std::string(*utfString);
}

v8::Local<v8::FunctionTemplate> ScriptSystem::GetTemplate(ObjectType type)
{
	if (_templates.find(type) != _templates.end())
	{
		return _templates[type].Get(_isolate);
	}

	return v8::Local<v8::FunctionTemplate>();
}

bool ScriptSystem::HasTemplate(ObjectType type) const
{
	return _templates.find(type) != _templates.end();
}

v8::Isolate* ScriptSystem::GetIsolate() const
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

	// Clean up wrappers
	Vec2Wrapper::DestroyTemplate();
	Vec3Wrapper::DestroyTemplate();
	TimeWrapper::DestroyTemplate();
	MapManagerWrapper::DestroyTemplate();
	FactoryWrapper::DestroyTemplate();

	delete _context;

	_isolate->Dispose();
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();

	delete _allocator;
}

void LogCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	if (info.Length() <= 0)
	{
		std::cout << "LOG: " << std::endl;
		return;
	}

	auto isolate = info.GetIsolate();
	v8::HandleScope handleScope(isolate);

	auto value = info[0];
	v8::String::Utf8Value stringValue(isolate, value);

	std::cout << "LOG: " << *stringValue << std::endl;
}