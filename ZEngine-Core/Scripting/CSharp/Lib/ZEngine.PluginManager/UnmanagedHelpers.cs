using System;
using System.Reflection;
using System.Runtime.Remoting;
using System.Runtime.InteropServices;

namespace ZEngine.Core.PluginManager {
  public static class UnmanagedHelpers {
    public static object UnwrapInstance(IntPtr objectPtr) {
      var handle = GCHandle.FromIntPtr(objectPtr);
      var instance = handle.Target;
      if (instance == null) throw new Exception("objectPtr is null");

      if (instance is ObjectHandle) {
        instance = ((ObjectHandle)instance).Unwrap();
      }
      if (instance == null) throw new Exception("objectPtr is null after unwrapping");
      
      return instance;
    }
  }
}
