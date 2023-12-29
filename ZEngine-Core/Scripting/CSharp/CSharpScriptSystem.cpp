#include <iostream>

#include "CSharpScriptSystem.h"
#include "DotnetRuntime.h"
#include "AssemblyLoader.h"

#if TARGET_OS_IPHONE
#include <dlfcn.h>
#endif

#define RUNTIME_CONFIG_PATH "ZEngine-Core.runtimeconfig.json"

#define INTEROP_ASSEMBLY_PATH "lib/ZEngine.Interop/ZEngine.Interop.dll"
#define INTEROP_ASSEMBLY_NAME "ZEngine.Interop"
#define INTEROP_ENTRYPOINT_NAMESPACE "ZEngine.Core.Interop"

#define PLUGIN_MANAGER_ASSEMBLY_PATH "lib/ZEngine.PluginManager/ZEngine.PluginManager.dll"
#define PLUGIN_MANAGER_ASSEMBLY_NAME "ZEngine.PluginManager"
#define PLUGIN_MANAGER_ENTRYPOINT_NAMESPACE "ZEngine.Core.PluginManager"

#define UNMANAGED_METHODS_TYPE_NAME "UnmanagedMethods"
#define CREATE_MANAGED_OBJECT_METHOD_NAME "CreateManagedObject"
#define INVOKE_METHOD_NAME "InvokeMethod"
#define SET_PROPERTY_METHOD_NAME "SetProperty"
#define SET_SCRIPT_NATIVE_INSTANCE_METHOD_NAME "SetScriptNativeInstance"
#define BUILD_PROJECT_METHOD_NAME "BuildProject"
#define LOAD_PROJECT_ASSEMBLY_METHOD_NAME "LoadProjectAssembly"
#define REGISTER_ADDITIONAL_DEPENDENCY_PATH_METHOD_NAME "RegisterAdditionalDependencyPath"

bool CSharpScriptSystem::Init()
{
#if !TARGET_OS_IPHONE
  DotnetRuntime::GetInstance()->Initialize(RUNTIME_CONFIG_PATH);
  LoadPluginManagerAssembly();
#else
  // C# code is natively compiled on iOS, so we need to load it from a shared library instead.
  const char* interopDllPath = "ZEngine-Interop.framework/ZEngine-Interop";
  _libraryHandle = dlopen(interopDllPath, RTLD_NOW);
  if (!_libraryHandle) {
    std::cerr << "Failed to load interop assembly: " << dlerror() << std::endl;
    throw std::runtime_error("Failed to load interop assembly");
  }

  _createManagedObjectFunction = reinterpret_cast<CreateManagedObjectFunction>(dlsym(_libraryHandle, "ZEngine_Core_Interop_UnmanagedMethods_CreateManagedObject"));
  _invokeMethodFunction = reinterpret_cast<InvokeMethodFunction>(dlsym(_libraryHandle, "ZEngine_Core_Interop_UnmanagedMethods_InvokeMethod"));
  _setPropertyFunction = reinterpret_cast<SetPropertyFunction>(dlsym(_libraryHandle, "ZEngine_Core_Interop_UnmanagedMethods_SetProperty"));
  _setScriptNativeInstanceFunction = reinterpret_cast<SetScriptNativeInstanceFunction>(dlsym(_libraryHandle, "ZEngine_Core_Interop_UnmanagedMethods_SetScriptNativeInstance"));
#endif
}

#if !TARGET_OS_IPHONE
void CSharpScriptSystem::HotReload()
{
  LoadProjectAssembly();
}

void CSharpScriptSystem::LoadPluginManagerAssembly()
{
  // NOTE: Only call AssemblyLoader::Load once per project.
  // Assemblies are loaded into their own isolated AssemblyLoadContext, so loading
  // multiple assemblies will result in multiple isolated contexts that cannot
  // reference/call each other.
  if (!_pluginManagerAssembly.Load(
    PLUGIN_MANAGER_ASSEMBLY_PATH,
    PLUGIN_MANAGER_ENTRYPOINT_NAMESPACE,
    PLUGIN_MANAGER_ASSEMBLY_NAME
  )) {
    throw std::runtime_error("Failed to load plugin manager assembly.");
  }

  // Get function pointers for interop methods from the ZEngine-Interop assembly.
  // TODO: Check the user script assembly references the ZEngine-Interop assembly.
  _createManagedObjectFunction = reinterpret_cast<CreateManagedObjectFunction>(
    _pluginManagerAssembly.GetFunction(
      UNMANAGED_METHODS_TYPE_NAME,
      PLUGIN_MANAGER_ENTRYPOINT_NAMESPACE,
      CREATE_MANAGED_OBJECT_METHOD_NAME,
      PLUGIN_MANAGER_ASSEMBLY_NAME
    )
  );
  _invokeMethodFunction = reinterpret_cast<InvokeMethodFunction>(
    _pluginManagerAssembly.GetFunction(
      UNMANAGED_METHODS_TYPE_NAME,
      PLUGIN_MANAGER_ENTRYPOINT_NAMESPACE,
      INVOKE_METHOD_NAME,
      PLUGIN_MANAGER_ASSEMBLY_NAME
    )
  );
  _setPropertyFunction = reinterpret_cast<SetPropertyFunction>(
    _pluginManagerAssembly.GetFunction(
      UNMANAGED_METHODS_TYPE_NAME,
      PLUGIN_MANAGER_ENTRYPOINT_NAMESPACE,
      SET_PROPERTY_METHOD_NAME,
      PLUGIN_MANAGER_ASSEMBLY_NAME
    )
  );
  _setScriptNativeInstanceFunction = reinterpret_cast<SetScriptNativeInstanceFunction>(
    _pluginManagerAssembly.GetFunction(
      UNMANAGED_METHODS_TYPE_NAME,
      PLUGIN_MANAGER_ENTRYPOINT_NAMESPACE,
      SET_SCRIPT_NATIVE_INSTANCE_METHOD_NAME,
      PLUGIN_MANAGER_ASSEMBLY_NAME
    )
  );
  _buildProjectFunction = reinterpret_cast<BuildProjectFunction>(
    _pluginManagerAssembly.GetFunction(
      UNMANAGED_METHODS_TYPE_NAME,
      PLUGIN_MANAGER_ENTRYPOINT_NAMESPACE,
      BUILD_PROJECT_METHOD_NAME,
      PLUGIN_MANAGER_ASSEMBLY_NAME
    )
  );
  _loadProjectAssemblyFunction = reinterpret_cast<LoadProjectAssemblyFunction>(
    _pluginManagerAssembly.GetFunction(
      UNMANAGED_METHODS_TYPE_NAME,
      PLUGIN_MANAGER_ENTRYPOINT_NAMESPACE,
      LOAD_PROJECT_ASSEMBLY_METHOD_NAME,
      PLUGIN_MANAGER_ASSEMBLY_NAME
    )
  );
  _registerAdditionalDependencyPathFunction = reinterpret_cast<RegisterAdditionalDependencyPathFunction>(
    _pluginManagerAssembly.GetFunction(
      UNMANAGED_METHODS_TYPE_NAME,
      PLUGIN_MANAGER_ENTRYPOINT_NAMESPACE,
      REGISTER_ADDITIONAL_DEPENDENCY_PATH_METHOD_NAME,
      PLUGIN_MANAGER_ASSEMBLY_NAME
    )
  );

  RegisterAdditionalDependencyPath(INTEROP_ASSEMBLY_PATH);
}
#endif

void* CSharpScriptSystem::CreateManagedObject(std::string typeName)
{
  if (!_createManagedObjectFunction) {
    std::cerr << "CSharpScriptSystem::CreateManagedObject called before plugin assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to create managed object");
  }

  return _createManagedObjectFunction(typeName.c_str());
}

void* CSharpScriptSystem::InvokeMethod(void* object, std::string methodName)
{
  if (!_invokeMethodFunction) {
    std::cerr << "CSharpScriptSystem::InvokeMEthod called before plugin assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to invoke method");
  }

  return _invokeMethodFunction(object, methodName.c_str());
}

void CSharpScriptSystem::SetProperty(void* object, std::string propertyName, void* value)
{
  if (!_setPropertyFunction) {
    std::cerr << "CSharpScriptSystem::SetProperty called before plugin assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to set property");
  }

  _setPropertyFunction(object, propertyName.c_str(), value);
}

void CSharpScriptSystem::SetScriptNativeInstance(void* object, void* nativeInstance)
{
  if (!_setScriptNativeInstanceFunction) {
    std::cerr << "CSharpScriptSystem::SetScriptNativeInstance called before plugin assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to set script native instance");
  }

  _setScriptNativeInstanceFunction(object, nativeInstance);
}

#if !TARGET_OS_IPHONE
bool CSharpScriptSystem::BuildProject(std::string projectPath, std::string dllOutputPath)
{
  if (!_buildProjectFunction) {
    std::cerr << "CSharpScriptSystem::BuildProject called before plugin assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to build project");
  }

  return _buildProjectFunction(projectPath.c_str(), dllOutputPath.c_str());
}

void CSharpScriptSystem::LoadProjectAssembly(std::string assemblyPath)
{
  _currentProjectAssemblyPath = assemblyPath;
  LoadProjectAssembly();
}

void CSharpScriptSystem::LoadProjectAssembly()
{
  if (!_loadProjectAssemblyFunction) {
    std::cerr << "CSharpScriptSystem::LoadProjectAssembly called before plugin assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to load project assembly");
  }

  if (_currentProjectAssemblyPath.empty()) {
    std::cerr << "CSharpScriptSystem::LoadProjectAssembly called before project assembly path was set." << std::endl;
    throw std::runtime_error("Failed to load project assembly");
  }

  _loadProjectAssemblyFunction(_currentProjectAssemblyPath.c_str());
}

void CSharpScriptSystem::RegisterAdditionalDependencyPath(std::string path)
{
  if (!_registerAdditionalDependencyPathFunction) {
    std::cerr << "CSharpScriptSystem::RegisterAdditionalDependencyPath called before plugin assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to register additional dependency path");
  }

  _registerAdditionalDependencyPathFunction(path.c_str());
}
#endif

void CSharpScriptSystem::Shutdown() 
{
#if !TARGET_OS_IPHONE
  DotnetRuntime::GetInstance()->Shutdown();
#else
  dlclose(_libraryHandle);
#endif
}
