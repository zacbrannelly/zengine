#include "CSharpScriptAsset.h"
#include "../../Utilities/File.h"
#include "../../Misc/Factory.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;
using namespace nlohmann;

CSharpScriptAsset::CSharpScriptAsset(std::string name) : Asset(name, CSHARP_SCRIPT_ASSET) 
{
  RegisterDerivedType(CSHARP_SCRIPT_ASSET);
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

  // Parse the JSON data.
	json root;
	in >> root;
	in.close();

  // Create a script instance and deserialize it from the JSON data.
  _script = Factory::CreateInstance<CSharpScript>(GetName());
  root.get_to(*_script);

  SetPath(path);
  return true;
}

std::string CSharpScriptAsset::GetScriptPath() const
{
  File assetFile(GetPath());
  auto basePath = assetFile.GetDirectory();
  return basePath + _script->GetClassName() + ".cs";
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

ZObject* CSharpScriptAsset::CreateInstance(string name, ObjectType type)
{
	return new CSharpScriptAsset(name);
}

CSharpScriptAsset::~CSharpScriptAsset()
{
}
