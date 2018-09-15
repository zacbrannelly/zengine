#pragma once

#include "../Map/Objects/ZObject.h"
#include <include/v8.h>

class Script : public ZObject
{
public:
	Script(std::string name);
	~Script();

	bool CompileFromFile(std::string path);
	bool Compile(std::string& code);

	void Execute();

private:
	v8::Persistent<v8::UnboundScript> _scriptObj;

public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
};

