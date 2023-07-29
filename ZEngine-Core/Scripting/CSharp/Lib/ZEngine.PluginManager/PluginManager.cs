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

      // Copy the plugin to a temporary location (to avoid failures due to leftover references from previous builds)
      Guid guid = Guid.NewGuid();
      string tempPath = Path.Combine(Path.GetTempPath(), "ZEngine-Builds", guid.ToString());
      Directory.CreateDirectory(tempPath);

      // Copy the plugin to the temp directory
      string tempAssemblyPath = Path.Combine(tempPath, Path.GetFileName(assemblyPath));
      File.Copy(assemblyPath, tempAssemblyPath);

      // Create a new AssemblyLoadContext for each plugin to ensure plugins can be unloaded independently
      _loadContext = new PluginAssemblyLoadContext(tempPath, _additionalPaths.ToArray<string>());

      // Load the main assembly
      _pluginAssembly = _loadContext.LoadFromAssemblyPath(tempAssemblyPath);

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
