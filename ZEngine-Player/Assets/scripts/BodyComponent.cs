using System.Collections.Generic;
using ZEngine.Core;

namespace MyGame
{
  public class BodyComponent : CSharpScriptComponent
  {

    private Queue<(float, float)> _past = new Queue<(float, float)>();

    public void Update()
    {
      var position = Transform.GetPosition();
      _past.Enqueue((position.x, position.y));

      if (_past.Count > 5) 
      {
        var pos = _past.Dequeue();
        if (Child != null) 
        {
          Child.GetTransform().SetPosition(pos.Item1, pos.Item2, 0);
        }
      }
    }

    public Entity Child { get; set; }
  }
}
