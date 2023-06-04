using ZEngine.Core;

namespace MyGame
{
  public class RotateComponent : CSharpScriptComponent
  {
    private const float SPEED = 90.0f * 5;

    private Time _time;

    public void Init()
    {
      _time = Time.GetInstance();
    }

    public void Update()
    {
      var rotation = Transform.GetRotation();
      rotation.x += SPEED * (float)_time.GetDeltaTime();
      rotation.y += SPEED * (float)_time.GetDeltaTime();
    }
  }
}
