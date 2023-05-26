using ZEngine.Core;

namespace MyGame
{
  public class TestComponent : ScriptBehaviour
  {
    public void Init()
    {
      Console.WriteLine($"{Entity.GetTransform().GetPositionX()}");
      Entity.GetTransform().SetPosition(5, 5, 0);
    }
  }
}
