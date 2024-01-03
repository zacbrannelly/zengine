#include <ZEngine-Core/Math/AABB.h>
#include <ZEngine-Core/Math/Picker.h>
#include <vector>
#include <map>

class Editor;
class MapView;
class Camera;
class Entity;
class MeshRenderer;

class MapViewPicker
{
public:
  MapViewPicker(Editor* editor, MapView* mapView);
  ~MapViewPicker();

  void UpdateAABBs();
  Entity* Pick(int mouseX, int mouseY, const Camera* camera);

private:
  Picker _picker;
  Editor* _editor;
  MapView* _mapView;
  std::map<MeshRenderer*, AABB> _meshBoundingBoxMap;
};
