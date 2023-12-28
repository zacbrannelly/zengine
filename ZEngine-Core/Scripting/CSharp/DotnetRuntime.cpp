#include "DotnetRuntime.h"
#include <iostream>
#include <dlfcn.h>
#include <sstream>
#include <TargetConditionals.h>

DotnetRuntime::DotnetRuntime() : 
  _hostfxrLib(nullptr),
  _context(nullptr),
  _hostfxrInitializeForRuntimeConfig(nullptr),
  _hostfxrGetRuntimeDelegate(nullptr),
  _loadAssemblyAndGetFunctionPtr(nullptr),
  _hostfxrClose(nullptr),
  _loaded(false)
{
}

void DotnetRuntime::Initialize(std::string runtimeConfigPath)
{
#if !TARGET_OS_IPHONE
  _runtimeConfigPath = runtimeConfigPath;
  _hostfxrLibPath = GetHostFxrPath();

  LoadHostFxr(_hostfxrLibPath);
  InitializeRuntime();
#else
  std::cerr << "DotnetRuntime::Initialize called on iOS, this is not supported." << std::endl;
  throw std::runtime_error("Failed to initialize dotnet runtime");
#endif
}

void DotnetRuntime::InitializeRuntime()
{
#if !TARGET_OS_IPHONE
  // Prepare for hosting in the current process.
  InitializeHostFxrContext(_runtimeConfigPath);

  // Initialize the runtime & get the "load assembly and get function pointer" function pointer.
  int getLoadAssemblyAndGetFuncPtrResult = _hostfxrGetRuntimeDelegate(_context, hdt_load_assembly_and_get_function_pointer, (void**)&_loadAssemblyAndGetFunctionPtr);
  if (getLoadAssemblyAndGetFuncPtrResult != 0 || _loadAssemblyAndGetFunctionPtr == nullptr)
  {
    throw std::runtime_error("Failed to get hdt_load_assembly_and_get_function_pointer. Error code: " + getLoadAssemblyAndGetFuncPtrResult);
  }
#else
  std::cerr << "DotnetRuntime::InitializeRuntime called on iOS, this is not supported." << std::endl;
  throw std::runtime_error("Failed to initialize dotnet runtime");
#endif
}

void DotnetRuntime::Restart()
{
  CloseHostFxr();
  InitializeRuntime();
}

load_assembly_and_get_function_pointer_fn DotnetRuntime::GetLoadAssemblyAndGetFunctionPtr() const
{
  return _loadAssemblyAndGetFunctionPtr;
}

void DotnetRuntime::Shutdown()
{
#if !TARGET_OS_IPHONE
  CloseHostFxr();
  dlclose(_hostfxrLib);
#endif
}

std::string DotnetRuntime::GetHostFxrPath() const
{
#if !TARGET_OS_IPHONE
  // Pre-allocate a large buffer for the path to hostfxr
  char buffer[4096];
  size_t buffer_size = sizeof(buffer) / sizeof(char_t);

  int getHostfxrPathResult = get_hostfxr_path(buffer, &buffer_size, nullptr);

  if (getHostfxrPathResult < 0) {
    std::cerr << "get_hostfxr_path failed: " << std::hex << std::showbase << getHostfxrPathResult << std::endl;
    throw std::runtime_error("Failed to get hostfxr path");
  }

  return std::string(buffer);
#else
  std::cerr << "DotnetRuntime::GetHostFxrPath called on iOS, this is not supported." << std::endl;
  throw std::runtime_error("Failed to get hostfxr path");
#endif
}

bool DotnetRuntime::LoadHostFxr(std::string& hostfxrLibPath) 
{
#if !TARGET_OS_IPHONE
  _hostfxrLib = dlopen(hostfxrLibPath.c_str(), RTLD_NOW);
  
  if (!_hostfxrLib)
  {
    std::cerr << "Failed to load hostfxr from path: " << hostfxrLibPath << std::endl;
    return false;
  }

  // Get function pointer to initialise hostfxr.
  _hostfxrInitializeForRuntimeConfig = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(dlsym(_hostfxrLib, "hostfxr_initialize_for_runtime_config"));
  if (!_hostfxrInitializeForRuntimeConfig)
  {
    std::cerr << "Failed to get hostfxr_initialize_for_runtime_config" << std::endl;
    return false;
  }

  // Get function pointer to get the runtime delegate.
  _hostfxrGetRuntimeDelegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(dlsym(_hostfxrLib, "hostfxr_get_runtime_delegate"));
  if (!_hostfxrInitializeForRuntimeConfig)
  {
    std::cerr << "Failed to get hostfxr_get_runtime_delegate" << std::endl;
    return false;
  }

  _hostfxrClose = reinterpret_cast<hostfxr_close_fn>(dlsym(_hostfxrLib, "hostfxr_close"));
  if (!_hostfxrClose)
  {
    std::cerr << "Failed to get hostfxr_close" << std::endl;
    return false;
  }

  return true;
#else
  std::cerr << "DotnetRuntime::LoadHostFxr called on iOS, this is not supported." << std::endl;
  throw std::runtime_error("Failed to load hostfxr");
#endif
}

bool DotnetRuntime::InitializeHostFxrContext(std::string& runtimeConfigPath)
{
#if !TARGET_OS_IPHONE
  // Initialize hostfxr context (prepare for creating the CoreCLR runtime).
  int initRuntimeResult = _hostfxrInitializeForRuntimeConfig(runtimeConfigPath.c_str(), nullptr, &_context);
  if (initRuntimeResult != 0 && initRuntimeResult != 1)
  {
    std::cerr << "Failed to initialize hostfxr. Error code: " << initRuntimeResult << std::endl;
    return false;
  }

  // TODO: Does this work enough?
  hostfxr_set_runtime_property_value_fn test = reinterpret_cast<hostfxr_set_runtime_property_value_fn>(dlsym(_hostfxrLib, "hostfxr_set_runtime_property_value"));
  test(_context, "APP_PATHS",  "/Users/zacbrannelly/GitRepos/zengine/debug/ZEngine-Editor");

  return true;
#else
  std::cerr << "DotnetRuntime::InitializeHostFxrContext called on iOS, this is not supported." << std::endl;
  throw std::runtime_error("Failed to initialize hostfxr context");
#endif
}

void DotnetRuntime::CloseHostFxr() 
{
#if !TARGET_OS_IPHONE
  _hostfxrClose(_context);
  _context = nullptr;
  _loadAssemblyAndGetFunctionPtr = nullptr;
#endif
}
