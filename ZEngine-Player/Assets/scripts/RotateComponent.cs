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
      float rotate = SPEED * (float)_time.GetDeltaTime();
      Transform.Rotate(new vec3(0, rotate, rotate));
    }
  }
}
