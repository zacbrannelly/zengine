using System;
using System.Linq;

namespace ZEngine.Core {
  public class ObjectTypeResolver
  {
    internal static System.Collections.Generic.Dictionary<System.Type, ObjectType> objectTypeMapping = new System.Collections.Generic.Dictionary<System.Type, ObjectType>
    {
      { typeof(Entity), ObjectType.ENTITY },
      { typeof(Map), ObjectType.MAP },
      { typeof(CSharpScriptComponent), ObjectType.CSHARP_SCRIPT_COMPONENT },
      { typeof(Transform), ObjectType.TRANSFORM },
      { typeof(AudioSource), ObjectType.AUDIO_SOURCE },
      { typeof(RigidBody3D), ObjectType.RIGID_BODY_3D },
      { typeof(Camera), ObjectType.CAMERA },
      { typeof(Collider3D), ObjectType.COLLIDER_3D },
    };

    public static ObjectType ResolveObjectType<T>()
    {
      return objectTypeMapping[typeof(T)];
    }

    public static Type ResolveType(ObjectType type)
    {
      return objectTypeMapping.FirstOrDefault(x => x.Value == type).Key;
    }
  }
}
