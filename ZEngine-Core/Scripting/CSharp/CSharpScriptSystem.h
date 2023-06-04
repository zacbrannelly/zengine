#include "../../Misc/Singleton.h"
#include <string>
#include "AssemblyLoader.h"

typedef void* (*CreateManagedObjectFunction)(const char* assemblyName, const char* typeName);
typedef void* (*InvokeMethodFunction)(void* object, const char* methodName);
typedef void (*SetPropertyFunction)(void* object, const char* propertyName, void* value);
typedef void (*SetScriptNativeInstanceFunction)(void* object, void* nativeInstance);

class CSharpScriptSystem : public Singleton<CSharpScriptSystem>
{
public:
  bool Init();
  void Shutdown();

  void* InvokeMethod(void* object, std::string methodName);
  void* CreateManagedObject(std::string assemblyName, std::string typeName);
  void SetProperty(void* managedInstance, std::string propertyName, void* value);
  void SetScriptNativeInstance(void* managedInstance, void* nativeInstance);

private:
  AssemblyLoader _interopAssembly;
  AssemblyLoader _scriptsAssembly;
  CreateManagedObjectFunction _createManagedObjectFunction;
  InvokeMethodFunction _invokeMethodFunction;
  SetPropertyFunction _setPropertyFunction;
  SetScriptNativeInstanceFunction _setScriptNativeInstanceFunction;
};
