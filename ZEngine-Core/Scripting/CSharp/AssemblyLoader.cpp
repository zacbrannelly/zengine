#include "AssemblyLoader.h"

#include <sstream>
#include "DotnetRuntime.h"

using namespace ZEngine;

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
    _logger.LogError("GetFunction: Called before assembly was loaded.");
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
    _logger.LogError("GetFunction: Failed to get function pointer. Error code: " + std::to_string(getFunctionPointerResult));
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
    _logger.LogError("LoadAssembly: Failed to load the assembly. Error code: " + std::to_string(loadAssemblyResult));
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
