using ZEngine.Core;

namespace MyGame
{
  public class BoxCollider2D
  {
    public BoxCollider2D(Transform transform, float width, float height) 
    {
      Position = transform.GetPosition();
      Width = width;
      Height = height;
    }

    public bool Contains(float x, float y)
    {
      return x >= Position.x && x <= Position.x + Width && y >= Position.y && y <= Position.y + Height;
    }

    public bool Intersects(BoxCollider2D other)
    {
	    return (
        (Position.x - Width / 2 < other.Position.x + other.Width / 2 && Position.x - Width / 2 > other.Position.x - other.Width / 2) ||
        (Position.x + Width / 2 > other.Position.x - other.Width / 2 && Position.x + Width / 2 < other.Position.x + other.Width / 2)
      ) && (
        (Position.y - Height / 2 < other.Position.y + other.Height / 2 && Position.y - Height / 2 > other.Position.y - other.Height / 2) ||
        (Position.y + Height / 2 > other.Position.y - other.Height / 2 && Position.y + Height / 2 < other.Position.y + other.Height / 2)
      );
    }

    public vec3 Position { get; set; }

    public float Width { get; set; }
    public float Height { get; set; }
  }
}