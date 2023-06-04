#include <iostream>

#include "CSharpScriptSystem.h"
#include "AssemblyLoader.h"

#define INTEROP_ASSEMBLY_PATH "ZEngine-Interop.dll"
#define INTEROP_ASSEMBLY_NAME "ZEngine-Interop"
#define INTEROP_RUNTIME_CONFIG_PATH "ZEngine-Interop.runtimeconfig.json"
#define INTEROP_ENTRYPOINT_NAMESPACE "ZEngine.Core.Interop"

#define SCRIPTS_ASSEMBLY_PATH "ZEngine-Scripts.dll"
#define SCRIPTS_ASSEMBLY_NAME "ZEngine-Scripts"
#define SCRIPTS_ENTRYPOINT_NAMESPACE "ZEngine.Core.Scripts"

#define UNMANAGED_METHODS_TYPE_NAME "UnmanagedMethods"
#define CREATE_MANAGED_OBJECT_METHOD_NAME "CreateManagedObject"
#define INVOKE_METHOD_NAME "InvokeMethod"
#define SET_PROPERTY_METHOD_NAME "SetProperty"
#define SET_SCRIPT_NATIVE_INSTANCE_METHOD_NAME "SetScriptNativeInstance"

bool CSharpScriptSystem::Init()
{
  bool interopLoaded = _interopAssembly.Load(
    INTEROP_ASSEMBLY_PATH,
    INTEROP_RUNTIME_CONFIG_PATH,
    INTEROP_ENTRYPOINT_NAMESPACE,
    INTEROP_ASSEMBLY_NAME
  );

  if (interopLoaded) {
    // Get function pointers for interop functions.
    _createManagedObjectFunction = reinterpret_cast<CreateManagedObjectFunction>(
      _interopAssembly.GetFunction(
        UNMANAGED_METHODS_TYPE_NAME,
        INTEROP_ENTRYPOINT_NAMESPACE,
        CREATE_MANAGED_OBJECT_METHOD_NAME,
        INTEROP_ASSEMBLY_NAME
      )
    );
    _invokeMethodFunction = reinterpret_cast<InvokeMethodFunction>(
      _interopAssembly.GetFunction(
        UNMANAGED_METHODS_TYPE_NAME,
        INTEROP_ENTRYPOINT_NAMESPACE,
        INVOKE_METHOD_NAME,
        INTEROP_ASSEMBLY_NAME
      )
    );
    _setPropertyFunction = reinterpret_cast<SetPropertyFunction>(
      _interopAssembly.GetFunction(
        UNMANAGED_METHODS_TYPE_NAME,
        INTEROP_ENTRYPOINT_NAMESPACE,
        SET_PROPERTY_METHOD_NAME,
        INTEROP_ASSEMBLY_NAME
      )
    );
    _setScriptNativeInstanceFunction = reinterpret_cast<SetScriptNativeInstanceFunction>(
      _interopAssembly.GetFunction(
        UNMANAGED_METHODS_TYPE_NAME,
        INTEROP_ENTRYPOINT_NAMESPACE,
        SET_SCRIPT_NATIVE_INSTANCE_METHOD_NAME,
        INTEROP_ASSEMBLY_NAME
      )
    );
  }

  bool scriptsLoaded = _scriptsAssembly.Load(
    SCRIPTS_ASSEMBLY_PATH,
    INTEROP_RUNTIME_CONFIG_PATH,
    SCRIPTS_ENTRYPOINT_NAMESPACE,
    SCRIPTS_ASSEMBLY_NAME
  );

  return interopLoaded && scriptsLoaded;
}

void* CSharpScriptSystem::CreateManagedObject(std::string assemblyName, std::string typeName)
{
  if (!_createManagedObjectFunction) {
    std::cerr << "CSharpScriptSystem::CreateManagedObject called before interop assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to create managed object");
  }

  return _createManagedObjectFunction(assemblyName.c_str(), typeName.c_str());
}

void* CSharpScriptSystem::InvokeMethod(void* object, std::string methodName)
{
  if (!_invokeMethodFunction) {
    std::cerr << "CSharpScriptSystem::InvokeMEthod called before interop assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to invoke method");
  }

  return _invokeMethodFunction(object, methodName.c_str());
}

void CSharpScriptSystem::SetProperty(void* object, std::string propertyName, void* value)
{
  if (!_setPropertyFunction) {
    std::cerr << "CSharpScriptSystem::SetProperty called before interop assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to set property");
  }

  _setPropertyFunction(object, propertyName.c_str(), value);
}

void CSharpScriptSystem::SetScriptNativeInstance(void* object, void* nativeInstance)
{
  if (!_setScriptNativeInstanceFunction) {
    std::cerr << "CSharpScriptSystem::SetScriptNativeInstance called before interop assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to set script native instance");
  }

  _setScriptNativeInstanceFunction(object, nativeInstance);
}

void CSharpScriptSystem::Shutdown() 
{
  _interopAssembly.Unload();
  _scriptsAssembly.Unload();
}
