%module ZEngine

%{
  #include <Misc/Factory.h>
%}

%include "std_string.i"

%ignore Factory::Init;
%ignore Factory::RegisterType;
%ignore Factory::RegisterCopyType;
%ignore Factory::RegisterTypeImporter;

%typemap(cscode) Factory %{
  public static T CreateInstance<T>(string name) where T : ZObject
  {
    ObjectType type = ObjectTypeResolver.ResolveObjectType<T>();
    return CreateInstance<T>(name, type);
  }

  public static T CreateInstance<T>(string name, ObjectType type) where T : ZObject
  {
    global::System.IntPtr cPtr = ZEnginePINVOKE.Factory_CreateInstance(name, (int)type);
    return Activator.CreateInstance(typeof(T), System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, new object[] { cPtr, false }, null) as T;
  }

  public static T Copy<T>(string name, ZObject zObject) where T : ZObject
  {
    global::System.IntPtr cPtr = ZEnginePINVOKE.Factory_Copy(name, ZObject.getCPtr(zObject));
    return Activator.CreateInstance(typeof(T), System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, new object[] { cPtr, false }, null) as T;
  }
%}

%include "Factory.h"
