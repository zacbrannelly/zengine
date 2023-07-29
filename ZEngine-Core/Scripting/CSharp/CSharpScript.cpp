#include "CSharpScript.h"
#include "CSharpScriptSystem.h"

#include "../../Component/CSharpScriptComponent.h"

CSharpScript::CSharpScript(std::string name) : ZObject(name, CSHARP_SCRIPT)
{
}

void CSharpScript::SetClass(std::string classNamespace, std::string className)
{
  _namespace = classNamespace;
  _className = className;
  _fullyQualifiedTypeName = _namespace + "." + _className;
}

void* CSharpScript::CreateManagedObject(CSharpScriptComponent* owner) const
{
  const auto scriptSystem = CSharpScriptSystem::GetInstance();
  const auto instance = scriptSystem->CreateManagedObject(_fullyQualifiedTypeName);
  scriptSystem->SetScriptNativeInstance(instance, owner);

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

std::string CSharpScript::GetClassName() const
{
  return _className;
}

CSharpScript::~CSharpScript()
{
}