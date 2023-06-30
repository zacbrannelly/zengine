using System;
using System.IO;
using System.Reflection;
using System.Runtime.Loader;
using System.Collections.Generic;

namespace ZEngine.Core.PluginManager
{
  public class PluginAssemblyLoadContext : AssemblyLoadContext
  {
    private List<AssemblyDependencyResolver> _resolvers;

    public PluginAssemblyLoadContext(string pluginPath, string[] additionalPaths) : base(isCollectible: true)
    {
        _resolvers = new List<AssemblyDependencyResolver>();
        _resolvers.Add(new AssemblyDependencyResolver(pluginPath));
        foreach (var path in additionalPaths)
        {
          _resolvers.Add(new AssemblyDependencyResolver(Path.GetFullPath(path)));
        }
    }

    protected override Assembly Load(AssemblyName assemblyName)
    {
      foreach (var resolver in _resolvers)
      {
        var assemblyPath = resolver.ResolveAssemblyToPath(assemblyName);
        if (assemblyPath != null)
        {
          return LoadFromAssemblyPath(assemblyPath);
        }
      }

      return null;
    }

    protected override IntPtr LoadUnmanagedDll(string unmanagedDllName)
    {
      foreach (var resolver in _resolvers)
      {
        var libraryPath = resolver.ResolveUnmanagedDllToPath(unmanagedDllName);
        if (libraryPath != null)
        {
          return LoadUnmanagedDllFromPath(libraryPath);
        }
      }

      return IntPtr.Zero;
    }
  }
}
