using System.Runtime.InteropServices;
using System.Reflection;
using ZEngine.Core.Interop;

namespace ZEngine.Core {
  public static class ZObjectExtensions
  {
    public static T Cast<T>(this ZObject obj) where T : ZObject
    {
      // If the object is a CSharpScriptComponent, we need to get the managed instance.
      if (obj.GetType() == ObjectType.CSHARP_SCRIPT_COMPONENT) {
        var scriptComponent = new CSharpScriptComponent(ZObject.getCPtr(obj).Handle, false);
        return UnmanagedHelpers.UnwrapInstance(scriptComponent.GetManagedInstance()) as T;
      }

      // If the object is already the type we want, just return it.
      if (obj is T) {
        return obj as T;
      }

      // Otherwise, create a new instance of the type we want, passing in the unmanaged instance handle.
      return Activator.CreateInstance(typeof(T), BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance, null, new object[] { ZObject.getCPtr(obj).Handle, false }, null) as T;
    }
  }
}
