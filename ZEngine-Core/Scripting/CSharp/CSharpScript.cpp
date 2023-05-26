#include "CSharpScript.h"
#include "CSharpScriptSystem.h"

#include "../../Map/Objects/Entity.h"

CSharpScript::CSharpScript(std::string name) : ZObject(name, CSHARP_SCRIPT)
{
}

void CSharpScript::SetClass(std::string classNamespace, std::string className)
{
  _namespace = classNamespace;
  _className = className;
  _fullyQualifiedTypeName = _namespace + "." + _className;
}

void* CSharpScript::CreateManagedObject(Entity* entity) const
{
  const auto scriptSystem = CSharpScriptSystem::GetInstance();
  const auto instance = scriptSystem->CreateManagedObject("ZEngine-Scripts", _fullyQualifiedTypeName);
  scriptSystem->SetProperty(instance, "_entityPtr", entity);

  return instance;
}

void CSharpScript::InvokeMethod(void* managedInstance, std::string methodName)
{
  CSharpScriptSystem::GetInstance()->InvokeMethod(managedInstance, methodName);
}

ZObject* CSharpScript::CreateInstance(std::string name, ObjectType type)
{
  return new CSharpScript(name);
}

CSharpScript::~CSharpScript()
{
}