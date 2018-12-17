#include "ScriptAsset.h"
#include "../../Scripting/Script.h"

using namespace std;

ScriptAsset::ScriptAsset(string name) : Asset(name, ObjectType::SCRIPT_ASSET)
{
	_script = nullptr;
}

Asset* ScriptAsset::CreateInstance(string name)
{
	return new ScriptAsset(name);
}

bool ScriptAsset::Load(string path)
{
	if (_script == nullptr)
			_script = new Script(GetName());

	if (_script->CompileFromFile(path))
	{
		_script->Execute();
		SetPath(path);

		return true;
	}

	return false;
}

Script* ScriptAsset::GetScript() const
{
	return _script;
}

void ScriptAsset::Release()
{
	if (_script != nullptr)
		delete _script;
}

ScriptAsset::~ScriptAsset()
{
}
