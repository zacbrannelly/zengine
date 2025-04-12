#include "DotnetRuntime.h"
#include <dlfcn.h>
#include <sstream>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

using namespace ZEngine;

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
#if defined(__APPLE__) && !TARGET_OS_IPHONE
  _runtimeConfigPath = runtimeConfigPath;
  _hostfxrLibPath = GetHostFxrPath();

  LoadHostFxr(_hostfxrLibPath);
  InitializeRuntime();
#else
  _logger.LogError("Initialize: Called on iOS, this is not supported.");
  throw std::runtime_error("Failed to initialize dotnet runtime");
#endif
}

void DotnetRuntime::InitializeRuntime()
{
#if defined(__APPLE__) && !TARGET_OS_IPHONE
  // Prepare for hosting in the current process.
  InitializeHostFxrContext(_runtimeConfigPath);

  // Initialize the runtime & get the "load assembly and get function pointer" function pointer.
  int getLoadAssemblyAndGetFuncPtrResult = _hostfxrGetRuntimeDelegate(_context, hdt_load_assembly_and_get_function_pointer, (void**)&_loadAssemblyAndGetFunctionPtr);
  if (getLoadAssemblyAndGetFuncPtrResult != 0 || _loadAssemblyAndGetFunctionPtr == nullptr)
  {
    _logger.LogError("InitializeRuntime: Failed to get function pointer. Error code: " + std::to_string(getLoadAssemblyAndGetFuncPtrResult));
    throw std::runtime_error("Failed to get hdt_load_assembly_and_get_function_pointer. Error code: " + getLoadAssemblyAndGetFuncPtrResult);
  }
#else
  _logger.LogError("InitializeRuntime: Called on iOS, this is not supported.");
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
#if defined(__APPLE__) && !TARGET_OS_IPHONE
  CloseHostFxr();
  dlclose(_hostfxrLib);
#endif
}

std::string DotnetRuntime::GetHostFxrPath()
{
#if defined(__APPLE__) && !TARGET_OS_IPHONE
  // Pre-allocate a large buffer for the path to hostfxr
  char buffer[4096];
  size_t buffer_size = sizeof(buffer) / sizeof(char_t);

  int getHostfxrPathResult = get_hostfxr_path(buffer, &buffer_size, nullptr);

  if (getHostfxrPathResult < 0) {
    _logger.LogError("GetHostFxrPath: Failed to get hostfxr path. Error code: " + std::to_string(getHostfxrPathResult));
    throw std::runtime_error("Failed to get hostfxr path");
  }

  return std::string(buffer);
#else
  _logger.LogError("GetHostFxrPath: Called on iOS, this is not supported.");
  throw std::runtime_error("Failed to get hostfxr path");
#endif
}

bool DotnetRuntime::LoadHostFxr(std::string& hostfxrLibPath) 
{
#if defined(__APPLE__) && !TARGET_OS_IPHONE
  _hostfxrLib = dlopen(hostfxrLibPath.c_str(), RTLD_NOW);
  
  if (!_hostfxrLib)
  {
    _logger.LogError("LoadHostFxr: Failed to load hostfxr from path: " + hostfxrLibPath);
    return false;
  }

  // Get function pointer to initialise hostfxr.
  _hostfxrInitializeForRuntimeConfig = reinterpret_cast<hostfxr_initialize_for_runtime_config_fn>(dlsym(_hostfxrLib, "hostfxr_initialize_for_runtime_config"));
  if (!_hostfxrInitializeForRuntimeConfig)
  {
    _logger.LogError("LoadHostFxr: Failed to get hostfxr_initialize_for_runtime_config");
    return false;
  }

  // Get function pointer to get the runtime delegate.
  _hostfxrGetRuntimeDelegate = reinterpret_cast<hostfxr_get_runtime_delegate_fn>(dlsym(_hostfxrLib, "hostfxr_get_runtime_delegate"));
  if (!_hostfxrInitializeForRuntimeConfig)
  {
    _logger.LogError("LoadHostFxr: Failed to get hostfxr_get_runtime_delegate");
    return false;
  }

  _hostfxrClose = reinterpret_cast<hostfxr_close_fn>(dlsym(_hostfxrLib, "hostfxr_close"));
  if (!_hostfxrClose)
  {
    _logger.LogError("LoadHostFxr: Failed to get hostfxr_close");
    return false;
  }

  return true;
#else
  _logger.LogError("LoadHostFxr: Called on iOS, this is not supported.");
  throw std::runtime_error("Failed to load hostfxr");
#endif
}

bool DotnetRuntime::InitializeHostFxrContext(std::string& runtimeConfigPath)
{
#if defined(__APPLE__) && !TARGET_OS_IPHONE
  // Initialize hostfxr context (prepare for creating the CoreCLR runtime).
  int initRuntimeResult = _hostfxrInitializeForRuntimeConfig(runtimeConfigPath.c_str(), nullptr, &_context);
  if (initRuntimeResult != 0 && initRuntimeResult != 1)
  {
    _logger.LogError("InitializeHostFxrContext: Failed to initialize hostfxr. Error code: " + std::to_string(initRuntimeResult));
    return false;
  }

  return true;
#else
  _logger.LogError("InitializeHostFxrContext: Called on iOS, this is not supported.");
  throw std::runtime_error("Failed to initialize hostfxr context");
#endif
}

void DotnetRuntime::CloseHostFxr() 
{
#if defined(__APPLE__) && !TARGET_OS_IPHONE
  _hostfxrClose(_context);
  _context = nullptr;
  _loadAssemblyAndGetFunctionPtr = nullptr;
#endif
}
