#include "CSharpScriptAsset.h"
#include "../../Scripting/CSharp/CSharpScript.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

CSharpScriptAsset::CSharpScriptAsset(std::string name) : Asset(name, CSHARP_SCRIPT_ASSET) 
{
  _script = nullptr;
}

bool CSharpScriptAsset::Load(std::string path)
{
  ifstream in(path, ios::in);

	if (!in.is_open())
	{
		cout << "CSHARP_SCRIPT_ASSET: Failed to load file: " << path << endl;
		return false;
	}

	json root;
	in >> root;
	in.close();

  string classNamespace;
  string className;

  _script = new CSharpScript(GetName());

  auto it = root.find("namespace");
  if (it != root.end() && (*it).is_string())
  {
    classNamespace = (*it).get<string>();
  }
  else
  {
    cerr << "CSHARP_SCRIPT_ASSET: Invalid script file, no namespace specified!" << endl;
    return false;
  }

  it = root.find("class");
  if (it != root.end() && (*it).is_string())
  {
    className = (*it).get<string>();
  }
  else
  {
    cerr << "CSHARP_SCRIPT_ASSET: Invalid script file, no class specified!" << endl;
    return false;
  }

  _script->SetClass(classNamespace, className);

  return true;
}

CSharpScript* CSharpScriptAsset::GetScript() const
{
  return _script;
}

void CSharpScriptAsset::Release()
{
  if (_script != nullptr)
  {
    delete _script;
    _script = nullptr;
  }
}

Asset* CSharpScriptAsset::CreateInstance(string name)
{
	return new CSharpScriptAsset(name);
}

CSharpScriptAsset::~CSharpScriptAsset()
{
}
