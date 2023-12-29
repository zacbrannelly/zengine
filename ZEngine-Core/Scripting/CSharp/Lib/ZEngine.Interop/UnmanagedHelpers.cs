using System;
using System.Reflection;
using System.Runtime.Remoting;
using System.Runtime.InteropServices;

namespace ZEngine.Core.Interop {
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

    public static bool ThrowIfNotSubclassOf(string baseTypeName, Type derivedType)
    {
        if (!IsSubclassOf(baseTypeName, derivedType))
        {
            throw new Exception("Type " + derivedType.Name + " is not a subclass of " + baseTypeName);
        }

        return true;
    }

    public static bool IsSubclassOf(string baseTypeName, Type derivedType)
    {
        // If the derivedType is null, then it cannot be a subclass of anything
        if (derivedType == null) return false;
        
        // Check if the derivedType is a subclass of the baseType
        if (derivedType.Name == baseTypeName) return true;

        // If not, recursively check if any of the base types of the derivedType
        // is a subclass of the baseType
        return IsSubclassOf(baseTypeName, derivedType.BaseType);
    }
  }
}
