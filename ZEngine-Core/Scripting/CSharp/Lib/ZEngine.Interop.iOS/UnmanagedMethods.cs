using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace ZEngine.Core.Interop 
{
  public static class UnmanagedMethods
  {
    [UnmanagedCallersOnly(EntryPoint = "ZEngine_Core_Interop_UnmanagedMethods_CreateManagedObject")]
    public static IntPtr CreateManagedObject(IntPtr typeNameString)
    {      
      var typeName = Marshal.PtrToStringAnsi(typeNameString);
      if (typeName == null) return IntPtr.Zero;

      var derp = Assembly.GetExecutingAssembly().GetTypes();
      Console.WriteLine("Types:");
      foreach (var foundType in derp) {
        Console.WriteLine(foundType.Name);
      }

      var type = Assembly.GetExecutingAssembly().GetType(typeName);
      var instance = Activator.CreateInstance(type);
      var handle = GCHandle.Alloc(instance);
      return GCHandle.ToIntPtr(handle);
    }

    [UnmanagedCallersOnly(EntryPoint = "ZEngine_Core_Interop_UnmanagedMethods_InvokeMethod")]
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

    [UnmanagedCallersOnly(EntryPoint = "ZEngine_Core_Interop_UnmanagedMethods_SetScriptNativeInstance")]
    public static void SetScriptNativeInstance(IntPtr managedInstancePtr, IntPtr unmanagedInstancePtr) {
      var instance = UnmanagedHelpers.UnwrapInstance(managedInstancePtr);
      if (instance == null) throw new Exception("Instance is null");

      UnmanagedHelpers.ThrowIfNotSubclassOf("CSharpScriptComponent", instance.GetType());

      var type = instance.GetType();
      var methodInfo = type.GetMethod("SetCPtr", BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);

      if (methodInfo == null) throw new Exception("SetCPtr method not found");
      methodInfo.Invoke(instance, new object[] { unmanagedInstancePtr, false });
    }

    [UnmanagedCallersOnly(EntryPoint = "ZEngine_Core_Interop_UnmanagedMethods_SetProperty")]
    public static void SetProperty(IntPtr objectPtr, IntPtr propertyNameString, IntPtr value) {
      var instance = UnmanagedHelpers.UnwrapInstance(objectPtr);

      var propertyName = Marshal.PtrToStringAnsi(propertyNameString);
      if (propertyName == null) throw new Exception("Property name is null");

      var propertyInfo = instance.GetType().GetProperty(propertyName, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
      if (propertyInfo == null) return;

      propertyInfo.SetValue(instance, value);
    }
  }
}
