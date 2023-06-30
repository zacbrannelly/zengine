using System;
using System.IO;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.Loader;

namespace ZEngine.Core.PluginManager
{
  public class PluginManager : IDisposable
  {
    private PluginAssemblyLoadContext _loadContext;
    private Assembly _pluginAssembly;
    private List<string> _additionalPaths = new List<string>();

    public void RegisterAdditionalPath(string path)
    {
      _additionalPaths.Add(path);
    }
    
    public Assembly LoadPlugin(string assemblyPath)
    {
      if (_loadContext != null)
      {
        UnloadPlugin();
      }

      assemblyPath = Path.GetFullPath(assemblyPath);
      Console.WriteLine("Loading plugin from path: " + assemblyPath);

      // Get directory from assembly path
      string pluginFolder = Path.GetDirectoryName(assemblyPath);

      // Create a new AssemblyLoadContext for each plugin to ensure plugins can be unloaded independently
      _loadContext = new PluginAssemblyLoadContext(pluginFolder, _additionalPaths.ToArray<string>());

      // Load the main assembly
      _pluginAssembly = _loadContext.LoadFromAssemblyPath(assemblyPath);

      return _pluginAssembly;
    }
    
    public void UnloadPlugin()
    {
      _loadContext?.Unload();
      _loadContext = null;
    }

    public void Dispose()
    {
      UnloadPlugin();
    }

    public Assembly PluginAssembly => _pluginAssembly;

    public static PluginManager Instance { get; private set; } = new PluginManager();
  }
}
