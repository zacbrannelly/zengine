#include <string>
#include "../../Misc/Singleton.h"
#include "Dotnet/nethost.h"
#include "Dotnet/hostfxr.h"
#include "Dotnet/coreclr_delegates.h"

namespace ZEngine
{
  class DotnetRuntime : public Singleton<DotnetRuntime>
  {
  public:
    DotnetRuntime();

    void Initialize(std::string runtimeConfigPath);
    void Restart();
    void Shutdown();

    load_assembly_and_get_function_pointer_fn GetLoadAssemblyAndGetFunctionPtr() const;
  private:
    void InitializeRuntime();

    std::string GetHostFxrPath() const;
    bool LoadHostFxr(std::string& hostfxrLibPath);
    bool InitializeHostFxrContext(std::string& runtimeConfigPath);

    void CloseHostFxr();

    std::string _runtimeConfigPath;
    std::string _hostfxrLibPath;
    void* _hostfxrLib;
    hostfxr_handle _context;
    hostfxr_close_fn _hostfxrClose;
    hostfxr_initialize_for_runtime_config_fn _hostfxrInitializeForRuntimeConfig;
    hostfxr_get_runtime_delegate_fn _hostfxrGetRuntimeDelegate;
    load_assembly_and_get_function_pointer_fn _loadAssemblyAndGetFunctionPtr;
    bool _loaded;
  };
}
