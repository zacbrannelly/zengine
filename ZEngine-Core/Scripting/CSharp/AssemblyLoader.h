#pragma once

#include <string>

class AssemblyLoader
{
public:
  AssemblyLoader();

  bool Load(
    std::string assemblyPath,
    std::string entrypointNamespace,
    std::string assemblyName
  );

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

  std::string _assemblyPath;
  std::string _assemblyName;
  bool _loaded;
};
