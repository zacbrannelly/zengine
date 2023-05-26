namespace ZEngine.Core
{
  public class ScriptBehaviour
  {
    internal Entity? _entity;
    internal IntPtr _entityPtr { get; set;}

    public Entity Entity
    {
      get
      {
        if (_entity == null)
        {
          _entity = new Entity(_entityPtr);
        }
        return _entity;
      }
    }
  }
}
