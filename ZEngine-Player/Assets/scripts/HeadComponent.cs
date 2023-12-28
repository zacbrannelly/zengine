using System.Collections.Generic;
using ZEngine.Core;

namespace MyGame
{
  public class HeadComponent : CSharpScriptComponent
  {
    private Time _time;
    private InputManager _inputManager;

    private FoodComponent _food;
    private List<Entity> _body;
    private BoxCollider2D _collider;

    public void Init()
    {
      _time = Time.GetInstance();
      _inputManager = InputManager.GetInstance();

      var scale = Transform.GetScale();
      _collider = new BoxCollider2D(Transform, scale.x * 2, scale.y * 2);

      _body = new List<Entity> { Entity };
      _food = MapManager
        .GetInstance()
        .GetCurrentMap()
        .Find("Food")
        .GetComponentByName("FoodComponent")
        .Cast<FoodComponent>();
    }

    public void Update()
    {
      if (_inputManager.GetButtonDown(ButtonCode.BUTTON_KEY_A))
      {
        SpawnBody();
      }

      if (_food.Collider.Intersects(_collider)) 
      {
        _food.Eat();
        SpawnBody();
      }
    }

    public void SpawnBody()
    {
      var source = _body.Last();
      var copy = Factory.Copy<Entity>($"Body {_body.Count}", source);

      var headComp = copy.GetComponentByName("HeadComponent");
      if (headComp != null) 
      {
        copy.RemoveComponent(headComp);
      }

      MapManager.GetInstance().GetCurrentMap().Add(copy);
      _body.Add(copy);

      // Link with the last body part (so it follows the head)
      var bodyComp = source.GetComponentByName("BodyComponent").Cast<BodyComponent>();
      bodyComp.Child = copy;
    }
  }
}
