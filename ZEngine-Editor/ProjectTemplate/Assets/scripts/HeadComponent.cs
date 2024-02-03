using System.Collections.Generic;
using ZEngine.Core;

namespace MyGame
{
  public class HeadComponent : CSharpScriptComponent
  {
    private Time _time;
    private InputManager _inputManager;
    private Physics3DSystem _physics;
    private Camera _camera;

    private FoodComponent _food;
    private List<Entity> _body;
    private BoxCollider2D _collider;

    private RigidBody3D _rigidBody;

    public void Init()
    {
      _time = Time.GetInstance();
      _inputManager = InputManager.GetInstance();
      _physics = Physics3DSystem.GetInstance();

      var map = MapManager.GetInstance().GetCurrentMap();
      _camera = map.GetCameras()[0];

      var scale = Transform.GetScale();
      _collider = new BoxCollider2D(Transform, scale.x * 2, scale.y * 2);

      _rigidBody = GetOwner().GetComponent<RigidBody3D>();

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
      // Spawn testing
      if (_inputManager.GetButtonDown(ButtonCode.BUTTON_KEY_A) || _inputManager.GetTouchDown(0))
      {
        // SpawnBody();
      }

      // Raycast testing
      var touchDown = _inputManager.GetTouchUp(0);
      if (_inputManager.GetButtonUp(ButtonCode.BUTTON_MOUSE_LEFT) || touchDown)
      {
        // Get mouse / touch coordinates
        var mousePos = touchDown ? _inputManager.GetTouchPos(0) : _inputManager.GetMousePos();
        int mouseX = (int)mousePos.x;
        int mouseY = (int)mousePos.y;

        Console.WriteLine($"Mouse pos: {mouseX}, {mouseY}");

        // Raycast into the scene (will only hit Collider3D objects)
        var ray = Ray.FromScreenPos(mouseX, mouseY, _camera);
        var result = new RaycastResult();
        if (_physics.Raycast(ray, 100, result))
        {
          // TODO: Investigate why this can be null.
          if (result.collider == null)
          {
            Console.WriteLine("Raycast hit nothing");
            return;
          }

          Console.WriteLine($"Raycast hit {result.collider.GetOwner().GetName()}");

          // TODO: Fix bug causing owner comparison `==` to fail.
          if (result.collider.GetOwner().GetName() == "Head")
          {
            Console.WriteLine("Raycast hit self");
            _rigidBody.ApplyImpulse(new vec3(0.1f, 0.1f, 0));
          }
        }
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
