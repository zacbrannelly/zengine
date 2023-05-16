#include "Script.h"
#include "ScriptSystem.h"
#include <iostream>
#include <fstream>

Script::Script(std::string name) : ZObject(name, ObjectType::SCRIPT)
{
}

bool Script::CompileFromFile(std::string path)
{
	std::ifstream in(path, std::ios::in);
	if (in.is_open())
	{
		std::string code((std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()));
		return Compile(code);
	}
	else
		std::cout << "SCRIPT: Failed to open file for compilation for script: " << GetName() << std::endl;

	return false;
}

bool Script::Compile(std::string& code)
{
	auto scriptSys = ScriptSystem::GetInstance();
	auto isolate = scriptSys->GetIsolate();
	auto context = scriptSys->GetContext()->GetLocal();

	v8::Context::Scope scope(context);

	auto source = v8::String::NewFromUtf8(isolate, code.c_str()).ToLocalChecked();
	auto script = v8::Script::Compile(context, source).ToLocalChecked();

	if (script.IsEmpty())
	{
		std::cout << "SCRIPT: Failed to compile script '" << GetName() << "'" << std::endl;
		return false;
	}

	_scriptObj.Reset(isolate, script->GetUnboundScript());

	return true;
}

void Script::Execute()
{
	auto scriptSys = ScriptSystem::GetInstance();
	auto context = scriptSys->GetContext()->GetLocal();

	// Create an execution scope for the script
	v8::Context::Scope scope(context);

	// Bind the script to the current context
	auto localScript = _scriptObj.Get(scriptSys->GetIsolate())->BindToCurrentContext();

	// Execute the script
	localScript->Run(context);
}

ZObject* Script::CreateInstance(std::string name, ObjectType type)
{
	return new Script(name);
}

Script::~Script()
{
	_scriptObj.Reset();
}
