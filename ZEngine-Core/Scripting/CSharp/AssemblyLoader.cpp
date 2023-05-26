#include <iostream>
#include <dlfcn.h>
#include <sstream>
#include "AssemblyLoader.h"

#define ENTRYPOINT_TYPE_NAME "Entrypoint"
#define ENTRYPOINT_METHOD_NAME "Initialize"

typedef void (*ExecuteDelegate)();

AssemblyLoader::AssemblyLoader() : hostfxrLib(nullptr), context(nullptr), hostfxrClose(nullptr), loaded(false)
{
}

bool AssemblyLoader::Load(
  std::string assemblyPath,
  std::string runtimeConfigPath,
  std::string entrypointNamespace,
  std::string assemblyName
)
{
  this->assemblyPath = assemblyPath;
  this->assemblyName = assemblyName;

  std::string hostfxrLibPath;
  try {
    hostfxrLibPath = GetHostFxrPath();
  } catch (const std::runtime_error& e) {
    std::cerr << "Failed to get hostfxr path: " << e.what() << std::endl;
    return false;
  }

  if (!LoadHostFxr(hostfxrLibPath)) {
    return false;
  }

  if (!InitializeHostFxr(runtimeConfigPath)) {
    return false;
  }

  if (!LoadAssembly(assemblyPath, entrypointNamespace, assemblyName)) {
    return false;
  }

  // Get function pointer to close the hostfxr context.
  hostfxrClose = reinterpret_cast<hostfxr_close_fn>(dlsym(hostfxrLib, "hostfxr_close"));

  // Mark the assembly as loaded.
  loaded = true;

  return true;
}

void* AssemblyLoader::GetFunction(std::string typeName, std::string owningNamespace, std::string methodName) {
  return GetFunction(typeName, owningNamespace, methodName, assemblyName);
}

void* AssemblyLoader::GetFunction(std::string typeName, std::string owningNamespace, std::string methodName, std::string assemblyName)
{
  if (!loaded) {
    std::cerr << "AssemblyLoader::GetFunction called before assembly was loaded." << std::endl;
    throw std::runtime_error("Failed to get function pointer");
  }

  // Generate fully qualified type name e.g. "Namespace.ClassName, AssemblyName"
  std::string fullTypeName = GenerateFullyQualifiedTypeName(typeName, owningNamespace, assemblyName);

  void* functionPointer = nullptr;
  // Load the assembly.
  int getFunctionPointerResult = loadAssemblyAndGetFunctionPtr(
    assemblyPath.c_str(),
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


std::string AssemblyLoader::GetHostFxrPath() const {
  // Pre-allocate a large buffer for the path to hostfxr
  char buffer[4096];
  size_t buffer_size = sizeof(buffer) / sizeof(char_t);

  int getHostfxrPathResult = get_hostfxr_path(buffer, &buffer_size, nullptr);

  if (getHostfxrPathResult < 0) {
    std::cerr << "get_hostfxr_path failed: " << std::hex << std::showbase << getHostfxrPathResult << std::endl;
    throw std::runtime_error("Failed to get hostfxr path");
  }

  return std::string(buffer);
}

bool AssemblyLoader::LoadHostFxr(std::string& hostfxrLibPath) 
{
  hostfxrLib = dlopen(hostfxrLibPath.c_str(), RTLD_NOW);
  
  if (!hostfxrLib)
  {
    std::cerr << "Failed to load hostfxr from path: " << hostfxrLibPath << std::endl;
    return false;
  }

  return true;
}

bool AssemblyLoader::InitializeHostFxr(std::string& runtimeConfigPath) {
  // Get function pointer to initialise hostfxr.
  hostfxr_initialize_for_runtime_config_fn initRuntime = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(dlsym(hostfxrLib, "hostfxr_initialize_for_runtime_config"));
  if (!initRuntime)
  {
    std::cerr << "Failed to get hostfxr_initialize_for_runtime_config" << std::endl;
    return false;
  }

  // Initialize hostfxr.
  int initRuntimeResult = initRuntime(runtimeConfigPath.c_str(), nullptr, &context);
  if (initRuntimeResult != 0 && initRuntimeResult != 1)
  {
    std::cerr << "Failed to initialize hostfxr. Error code: " << initRuntimeResult << std::endl;
    return false;
  }

  return true;
}

bool AssemblyLoader::LoadAssembly(
  std::string& assemblyPath,
  std::string& entrypointNamespace,
  std::string& assemblyName
)
{
  hostfxr_get_runtime_delegate_fn hostfxr_get_runtime_delegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(dlsym(hostfxrLib, "hostfxr_get_runtime_delegate"));

  // Get function pointer for 'load_assembly_and_get_function_pointer' from hostfxr.
  int getLoadAssemblyAndGetFuncPtrResult = hostfxr_get_runtime_delegate(context, hdt_load_assembly_and_get_function_pointer, (void**)&loadAssemblyAndGetFunctionPtr);
  if (getLoadAssemblyAndGetFuncPtrResult != 0 || loadAssemblyAndGetFunctionPtr == nullptr)
  {
    std::cerr << "Failed to get hdt_load_assembly_and_get_function_pointer. Error code: " << getLoadAssemblyAndGetFuncPtrResult << std::endl;
    return false;
  }

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

  return true;
}

std::string AssemblyLoader::GenerateFullyQualifiedTypeName(std::string typeName, std::string& owningNamespace, std::string& assemblyName) {
  std::stringstream fullTypeName;
  fullTypeName << owningNamespace << "." << typeName << ", " << assemblyName;
  return fullTypeName.str();
}

void AssemblyLoader::Unload() {
  if (hostfxrClose != nullptr) {
    // Close the hostfxr context.
    hostfxrClose(context);

    // Unload the nethost library
    dlclose(hostfxrLib);

    hostfxrClose = nullptr;
    context = nullptr;
    hostfxrLib = nullptr;
  }
}

AssemblyLoader::~AssemblyLoader() 
{
  Unload();
}