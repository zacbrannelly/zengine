#pragma once

#include <map>
#include <string>
#include "../../Utilities/JsonHelpers.h"
#include "../../Map/Objects/ZObject.h"

class CSharpScriptComponent;

class CSharpScript : public ZObject
{
public:
  CSharpScript(std::string name);
  ~CSharpScript();

  void SetNamespace(std::string classNamespace);
  void SetClassName(std::string className);
  void SetClass(std::string classNamespace, std::string className);
  std::string GetClassName() const;

  void* CreateManagedObject(CSharpScriptComponent* owner) const;
  void InvokeMethod(void* managedInstance, std::string methodName);

private:
  std::string _namespace;
  std::string _className;
  std::string _fullyQualifiedTypeName;
  std::map<std::string, void*> _methodsCache;

public:
  static ZObject* CreateInstance(std::string name, ObjectType type);

  static ObjectType GetStaticType()
  {
    return CSHARP_SCRIPT;
  }

  // Allow serialization / deserialization
  JSON_SCHEMA_BEGIN(CSharpScript)
    JSON_MAP_TO_SETTER (namespace, SetNamespace, std::string)
    JSON_MAP_TO_SETTER (class,     SetClassName, std::string)
  JSON_SCHEMA_END()
};
