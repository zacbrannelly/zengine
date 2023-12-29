#include "../../Misc/Singleton.h"
#include <string>
#include <TargetConditionals.h>
#include "AssemblyLoader.h"

typedef void* (*CreateManagedObjectFunction)(const char* typeName);
typedef void* (*InvokeMethodFunction)(void* object, const char* methodName);
typedef void (*SetPropertyFunction)(void* object, const char* propertyName, void* value);
typedef void (*SetScriptNativeInstanceFunction)(void* object, void* nativeInstance);
typedef bool (*BuildProjectFunction)(const char* projectPath, const char* dllOutputPath);
typedef void (*LoadProjectAssemblyFunction)(const char* assemblyPath);
typedef void (*RegisterAdditionalDependencyPathFunction)(const char* path);

class CSharpScriptSystem : public Singleton<CSharpScriptSystem>
{
public:
  bool Init();
  void Shutdown();

  void* InvokeMethod(void* object, std::string methodName);
  void* CreateManagedObject(std::string typeName);
  void SetProperty(void* managedInstance, std::string propertyName, void* value);
  void SetScriptNativeInstance(void* managedInstance, void* nativeInstance);
  
#if !TARGET_OS_IPHONE
  void HotReload();

  bool BuildProject(std::string projectPath, std::string dllOutputPath);
  void LoadProjectAssembly(std::string assemblyPath);
  void RegisterAdditionalDependencyPath(std::string path);
#endif

private:
#if !TARGET_OS_IPHONE
  void LoadPluginManagerAssembly();
  void LoadProjectAssembly();
#endif

  std::string _currentProjectAssemblyPath;

  AssemblyLoader _pluginManagerAssembly;
  CreateManagedObjectFunction _createManagedObjectFunction;
  InvokeMethodFunction _invokeMethodFunction;
  SetPropertyFunction _setPropertyFunction;
  SetScriptNativeInstanceFunction _setScriptNativeInstanceFunction;

#if !TARGET_OS_IPHONE
  BuildProjectFunction _buildProjectFunction;
  LoadProjectAssemblyFunction _loadProjectAssemblyFunction;
  RegisterAdditionalDependencyPathFunction _registerAdditionalDependencyPathFunction;
#else
  void* _libraryHandle;
#endif
};
