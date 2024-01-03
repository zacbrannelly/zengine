#include "MapViewPicker.h"
#include "../Editor.h"
#include <ZEngine-Core/Math/Ray.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Component/MeshRenderer.h>
#include <ZEngine-Core/Component/Transform.h>
#include <stack>
#include <set>

MapViewPicker::MapViewPicker(Editor* editor, MapView* mapView) : _editor(editor), _mapView(mapView)
{
}

void MapViewPicker::UpdateAABBs()
{
  const auto map = _editor->GetSelectedMap();
  if (map == nullptr)
  {
    _meshBoundingBoxMap.clear();
    return;
  }

  const auto& entities = map->GetEntities();
  std::set<MeshRenderer*> existingMeshes;

  for (Entity* entity : entities)
  {
    auto meshRenderers = entity->GetComponentsByType<MeshRenderer>();
    for (MeshRenderer* meshRenderer : meshRenderers)
    {
      existingMeshes.insert(meshRenderer);

      if (_meshBoundingBoxMap.contains(meshRenderer))
      {
        _meshBoundingBoxMap[meshRenderer].SetTransform(entity->GetTransform()->GetWorldTransformMatrix());
      } 
      else 
      {
        AABB boundingBox;
        boundingBox.SetFromMesh(meshRenderer->GetMesh(), entity->GetTransform()->GetWorldTransformMatrix());
        boundingBox.userData = entity;
        _meshBoundingBoxMap[meshRenderer] = boundingBox;
      }
    }
  }

  std::vector<MeshRenderer*> toRemove;
  for (auto& [meshRenderer, boundingBox] : _meshBoundingBoxMap)
  {
    if (existingMeshes.find(meshRenderer) == existingMeshes.end())
    {
      toRemove.push_back(meshRenderer);
    }
  }

  for (MeshRenderer* meshRenderer : toRemove)
  {
    _meshBoundingBoxMap.erase(meshRenderer);
  }
}

Entity* MapViewPicker::Pick(int mouseX, int mouseY, const Camera* camera)
{
  // TODO: Find a more efficient way to store the bounding boxes
  std::vector<AABB> boundingBoxes;
  for (auto& [meshRenderer, boundingBox] : _meshBoundingBoxMap)
  {
    boundingBoxes.push_back(boundingBox);
  }

  const auto mouseRay = _picker.GetRayFromScreenPos(mouseX, mouseY, camera);
  const auto pickedAABB = _picker.PickAABB(boundingBoxes, mouseRay);
  if (pickedAABB == nullptr) return nullptr;

  return static_cast<Entity*>(pickedAABB->userData);
}

MapViewPicker::~MapViewPicker()
{
}
