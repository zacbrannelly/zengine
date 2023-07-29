using ZEngine.Core;

namespace MyGame
{
  public class TestComponent : CSharpScriptComponent
  {
    private const float SPEED = 6.0f;

    private InputManager _inputManager;
    private Time _time;
    private vec3 _velocity = new vec3(SPEED, 0, 0);

    public void Init()
    {
      _inputManager = InputManager.GetInstance();
      _time = Time.GetInstance();
    }

    public void Update()
    {
      // Move according to input
      if (_inputManager.GetButtonDown(ButtonCode.BUTTON_KEY_A)) 
      {
        _velocity.x = -SPEED;
        _velocity.y = 0;
      }
      else if (_inputManager.GetButtonDown(ButtonCode.BUTTON_KEY_D))
      {
        _velocity.x = SPEED;
        _velocity.y = 0;
      }
	
      if (_inputManager.GetButtonDown(ButtonCode.BUTTON_KEY_W))
      {
        _velocity.x = 0;
        _velocity.y = SPEED;
      }
      else if (_inputManager.GetButtonDown(ButtonCode.BUTTON_KEY_S))
      {
        _velocity.x = 0;
        _velocity.y = -SPEED;
      }

      var position = Transform.GetPosition();
      position.x += _velocity.x * (float)_time.GetDeltaTime();
      position.y += _velocity.y * (float)_time.GetDeltaTime();
      position.z += _velocity.z * (float)_time.GetDeltaTime();
    }
  }
}
