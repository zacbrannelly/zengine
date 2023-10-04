using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace ZEngine.Core.PluginManager
{
  public class UnmanagedMethods
  {
    [UnmanagedCallersOnly]
    public static IntPtr CreateManagedObject(IntPtr typeNameString)
    {      
      var typeName = Marshal.PtrToStringAnsi(typeNameString);
      if (typeName == null) return IntPtr.Zero;

      var instance = PluginManager.Instance.PluginAssembly.CreateInstance(typeName);
      var handle = GCHandle.Alloc(instance);
      return GCHandle.ToIntPtr(handle);
    }

    [UnmanagedCallersOnly]
    public static IntPtr InvokeMethod(IntPtr objectPtr, IntPtr methodNameString) {
      var instance = UnmanagedHelpers.UnwrapInstance(objectPtr);

      var methodName = Marshal.PtrToStringAnsi(methodNameString);
      if (methodName == null) throw new Exception("Method name is null");

      var type = instance.GetType();
      var methodInfo = type.GetMethod(methodName, BindingFlags.Public | BindingFlags.Instance);
      if (methodInfo == null) return IntPtr.Zero;

      var result = methodInfo.Invoke(instance, null);
      if (result == null) return IntPtr.Zero;

      var resultHandle = GCHandle.Alloc(result);
      return GCHandle.ToIntPtr(resultHandle);
    }

    [UnmanagedCallersOnly]
    public static void SetScriptNativeInstance(IntPtr managedInstancePtr, IntPtr unmanagedInstancePtr) {
      var instance = UnmanagedHelpers.UnwrapInstance(managedInstancePtr);
      if (instance == null) throw new Exception("Instance is null");

      UnmanagedHelpers.ThrowIfNotSubclassOf("CSharpScriptComponent", instance.GetType());

      var type = instance.GetType();
      var methodInfo = type.GetMethod("SetCPtr", BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);

      if (methodInfo == null) throw new Exception("SetCPtr method not found");
      methodInfo.Invoke(instance, new object[] { unmanagedInstancePtr, false });
    }

    [UnmanagedCallersOnly]
    public static void SetProperty(IntPtr objectPtr, IntPtr propertyNameString, IntPtr value) {
      var instance = UnmanagedHelpers.UnwrapInstance(objectPtr);

      var propertyName = Marshal.PtrToStringAnsi(propertyNameString);
      if (propertyName == null) throw new Exception("Property name is null");

      var propertyInfo = instance.GetType().GetProperty(propertyName, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
      if (propertyInfo == null) return;

      propertyInfo.SetValue(instance, value);
    }

    [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
    public static int BuildProject(IntPtr projectPathString, IntPtr dllOutputPathString) {
      var projectPath = Marshal.PtrToStringAnsi(projectPathString);
      if (projectPath == null) throw new Exception("Project path is null");

      var dllOutputPath = Marshal.PtrToStringAnsi(dllOutputPathString);
      if (dllOutputPath == null) throw new Exception("DLL output path is null");

      try {
        return ProjectBuilder.Build(projectPath, dllOutputPath) ? 1 : 0;
      } catch (Exception e) {
        Console.WriteLine(e);
        return 0;
      }
    }

    [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
    public static void LoadProjectAssembly(IntPtr assemblyPathString)
    {
      var assemblyPath = Marshal.PtrToStringAnsi(assemblyPathString);
      if (assemblyPath == null) throw new Exception("Assembly path is null");
      PluginManager.Instance.LoadPlugin(assemblyPath);
    }

    [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvCdecl) })]
    public static void RegisterAdditionalDependencyPath(IntPtr pathString)
    {
      var path = Marshal.PtrToStringAnsi(pathString);
      if (path == null) throw new Exception("Path is null");
      PluginManager.Instance.RegisterAdditionalPath(path);
    }
  }
}
