using System.Collections.Generic;
using ZEngine.Core;

namespace MyGame
{
  public class FoodComponent : CSharpScriptComponent
  {
    private Random _random = new Random();
    private BoxCollider2D _collider;
    private AudioSource _audioSource;

    public void Init()
    {
      // Setup 2D collider
      var scale = Transform.GetScale();
      _collider = new BoxCollider2D(Transform, 0.4f, 0.4f);

      // Fetch audio source on this entity
      // TODO: Figure out why this is null sometimes.
      _audioSource = GetComponent<AudioSource>();
    }

    public void Eat() 
    {
      Transform.SetPosition(-5 + 10 * _random.NextSingle(), -5 + 10 * _random.NextSingle(), 0);
      if (_audioSource != null) _audioSource.Play();
    }

    public BoxCollider2D Collider => _collider;
  }
}