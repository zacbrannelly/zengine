#pragma once

#include <string>
#include "Dotnet/nethost.h"
#include "Dotnet/hostfxr.h"
#include "Dotnet/coreclr_delegates.h"

class AssemblyLoader
{
public:
  AssemblyLoader();
  ~AssemblyLoader();

  bool Load(
    std::string assemblyPath,
    std::string runtimeConfigPath,
    std::string entrypointNamespace,
    std::string assemblyName
  );
  void Unload();

  void* GetFunction(
    std::string typeName,
    std::string owningNamespace,
    std::string methodName
  );

  void* GetFunction(
    std::string typeName,
    std::string owningNamespace,
    std::string methodName,
    std::string assemblyName
  );

private:
  std::string GetHostFxrPath() const;
  bool LoadHostFxr(std::string& hostfxrLibPath);
  bool InitializeHostFxr(std::string& runtimeConfigPath);
  bool LoadAssembly(
    std::string& assemblyPath,
    std::string& entrypointNamespace,
    std::string& assemblyName
  );

  std::string GenerateFullyQualifiedTypeName(
    std::string typeName,
    std::string& owningNamespace,
    std::string& assemblyName
  );

  std::string assemblyPath;
  std::string assemblyName;
  void* hostfxrLib;
  hostfxr_handle context;
  hostfxr_close_fn hostfxrClose;
  load_assembly_and_get_function_pointer_fn loadAssemblyAndGetFunctionPtr;
  bool loaded;
};
