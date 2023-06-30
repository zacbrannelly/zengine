#include "AssemblyLoader.h"

#include <iostream>
#include <sstream>
#include "DotnetRuntime.h"

#define ENTRYPOINT_TYPE_NAME "Entrypoint"
#define ENTRYPOINT_METHOD_NAME "Initialize"

typedef void (*ExecuteDelegate)();

AssemblyLoader::AssemblyLoader() : _loaded(false)
{
}

bool AssemblyLoader::Load(
  std::string assemblyPath,
  std::string entrypointNamespace,
  std::string assemblyName
)
{
  _assemblyPath = assemblyPath;
  _assemblyName = assemblyName;

  if (!LoadAssembly(assemblyPath, entrypointNamespace, assemblyName)) {
    return false;
  }

  // Mark the assembly as loaded.
  _loaded = true;

  return true;
}

void* AssemblyLoader::GetFunction(std::string typeName, std::string owningNamespace, std::string methodName) {
  return GetFunction(typeName, owningNamespace, methodName, _assemblyName);
}

void* AssemblyLoader::GetFunction(std::string typeName, std::string owningNamespace, std::string methodName, std::string assemblyName)
{
  if (!_loaded) {
    std::cerr << "AssemblyLoader::GetFunction called before assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to get function pointer");
  }

  // Generate fully qualified type name e.g. "Namespace.ClassName, AssemblyName"
  std::string fullTypeName = GenerateFullyQualifiedTypeName(typeName, owningNamespace, assemblyName);

  // Load the assembly and ptr to the function.
  void* functionPointer = nullptr;
  auto loadAssemblyAndGetFunctionPtr = DotnetRuntime::GetInstance()->GetLoadAssemblyAndGetFunctionPtr();
  int getFunctionPointerResult = loadAssemblyAndGetFunctionPtr(
    _assemblyPath.c_str(),
    fullTypeName.c_str(),
    methodName.c_str(),
    UNMANAGEDCALLERSONLY_METHOD,
    nullptr,
    (void**)&functionPointer
  );

  if (getFunctionPointerResult < 0) {
    std::cerr << "get_function_pointer failed: " << std::hex << std::showbase << getFunctionPointerResult << std::endl;
    throw std::runtime_error("Failed to get function pointer");
  }

  return functionPointer;
}

bool AssemblyLoader::LoadAssembly(
  std::string& assemblyPath,
  std::string& entrypointNamespace,
  std::string& assemblyName
)
{
  auto loadAssemblyAndGetFunctionPtr = DotnetRuntime::GetInstance()->GetLoadAssemblyAndGetFunctionPtr();
  std::string fullEntrypointTypename = GenerateFullyQualifiedTypeName(ENTRYPOINT_TYPE_NAME, entrypointNamespace, assemblyName);

  // Load the assembly.
  ExecuteDelegate executeDelegate = nullptr;
  int loadAssemblyResult = loadAssemblyAndGetFunctionPtr(
    assemblyPath.c_str(),
    fullEntrypointTypename.c_str(),
    ENTRYPOINT_METHOD_NAME,
    UNMANAGEDCALLERSONLY_METHOD,
    nullptr,
    (void**)&executeDelegate
  );

  if (loadAssemblyResult != 0 || !executeDelegate)
  {
    std::cerr << "Failed to load the assembly. Error code: " << loadAssemblyResult << std::endl;
    return false;
  }

  // Call the entrypoint.
  executeDelegate();

  return true;
}

std::string AssemblyLoader::GenerateFullyQualifiedTypeName(std::string typeName, std::string& owningNamespace, std::string& assemblyName) {
  std::stringstream fullTypeName;
  fullTypeName << owningNamespace << "." << typeName << ", " << assemblyName;
  return fullTypeName.str();
}
