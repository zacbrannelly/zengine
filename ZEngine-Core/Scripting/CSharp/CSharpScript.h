#pragma once

#include <map>
#include <string>
#include "../../Map/Objects/ZObject.h"

class Entity;

class CSharpScript : public ZObject
{
public:
	CSharpScript(std::string name);
	~CSharpScript();

	void SetClass(std::string classNamespace, std::string className);

  void* CreateManagedObject(Entity* owner) const;
  void InvokeMethod(void* managedInstance, std::string methodName);

private:
  std::string _namespace;
  std::string _className;
  std::string _fullyQualifiedTypeName;
  std::map<std::string, void*> _methodsCache;

public:
	static ZObject* CreateInstance(std::string name, ObjectType type);
};
