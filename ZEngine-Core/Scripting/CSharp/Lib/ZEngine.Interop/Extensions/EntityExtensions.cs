namespace ZEngine.Core {
  public static class EntityExtensions {
    public static T? GetComponent<T>(this Entity entity) where T : Component {
      var objectType = ObjectTypeResolver.ResolveObjectType<T>();
      Component? component = entity.GetComponent(objectType);
      return component?.Cast<T>();
    }
  }
}
