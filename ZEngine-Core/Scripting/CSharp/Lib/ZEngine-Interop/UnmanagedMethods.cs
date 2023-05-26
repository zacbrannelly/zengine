using System;
using System.Reflection;
using System.Runtime.InteropServices;

namespace ZEngine.Core.Interop
{
  public class UnmanagedMethods
  {
    [UnmanagedCallersOnly]
    public static IntPtr CreateManagedObject(IntPtr assemblyNameString, IntPtr typeNameString)
    {
      var assemblyName = Marshal.PtrToStringAnsi(assemblyNameString);
      if (assemblyName == null) return IntPtr.Zero;

      var typeName = Marshal.PtrToStringAnsi(typeNameString);
      if (typeName == null) return IntPtr.Zero;

      var instance = Activator.CreateInstance(
        assemblyName,
        typeName,
        false,
        0,
        null,
        null,
        null,
        null
      );
      var handle = GCHandle.Alloc(instance);
      return GCHandle.ToIntPtr(handle);
    }

    [UnmanagedCallersOnly]
    public static IntPtr InvokeMethod(IntPtr objectPtr, IntPtr methodNameString) {
      var handle = GCHandle.FromIntPtr(objectPtr);
      var instance = handle.Target;
      if (instance == null) return IntPtr.Zero;

      var methodName = Marshal.PtrToStringAnsi(methodNameString);
      if (methodName == null) return IntPtr.Zero;

      // Unwrap the object handle if it's a proxy
      if (instance is System.Runtime.Remoting.ObjectHandle) {
        instance = ((System.Runtime.Remoting.ObjectHandle)instance).Unwrap();
      }

      var type = instance.GetType();
      var methodInfo = type.GetMethod(methodName, BindingFlags.Public | BindingFlags.Instance);
      if (methodInfo == null) return IntPtr.Zero;

      var result = methodInfo.Invoke(instance, null);
      if (result == null) return IntPtr.Zero;

      var resultHandle = GCHandle.Alloc(result);
      return GCHandle.ToIntPtr(resultHandle);
    }

    [UnmanagedCallersOnly]
    public static void SetProperty(IntPtr objectPtr, IntPtr propertyNameString, IntPtr value) {
      var handle = GCHandle.FromIntPtr(objectPtr);
      var instance = handle.Target;
      if (instance == null) return;

      // Unwrap the object handle if it's a proxy
      if (instance is System.Runtime.Remoting.ObjectHandle) {
        instance = ((System.Runtime.Remoting.ObjectHandle)instance).Unwrap();
      }

      var propertyName = Marshal.PtrToStringAnsi(propertyNameString);
      if (propertyName == null) return;

      var propertyInfo = instance.GetType().GetProperty(propertyName, BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
      if (propertyInfo == null) return;

      propertyInfo.SetValue(instance, value);
    }
  }
}
