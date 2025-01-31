#include "GizmoSystem.h"
#include "BoxCollider3DGizmoRenderer.h"
#include "SphereCollider3DGizmoRenderer.h"
#include "CapsuleCollider3DGizmoRenderer.h"
#include "../Editor.h"
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Component/Component.h>
#include <vector>

using namespace ZEngine;

void GizmoSystem::Init(Editor* editor)
{
  _editor = editor;

  // Register all gizmo renderers
  _objectTypeToRenderer[BOX_COLLIDER_3D] = new BoxCollider3DGizmoRenderer();
  _objectTypeToRenderer[SPHERE_COLLIDER_3D] = new SphereCollider3DGizmoRenderer();
  _objectTypeToRenderer[CAPSULE_COLLIDER_3D] = new CapsuleCollider3DGizmoRenderer();
}

void GizmoSystem::RenderGizmos(int viewId)
{
  Entity* selectedEntity = _editor->GetSelectedEntity();
  if (selectedEntity == nullptr) return;

  // Fetch all components of the selected entity and group them by type
  std::map<ObjectType, std::vector<Component*>> objectTypeToComponents;
  for (Component* component : selectedEntity->GetAllComponents())
  {
    objectTypeToComponents[component->GetType()].push_back(component);
  }

  // Render all components of the selected entity
  for (auto& pair : objectTypeToComponents)
  {
    auto it = _objectTypeToRenderer.find(pair.first);
    if (it != _objectTypeToRenderer.end())
    {
      it->second->Render(viewId, pair.second);
    }
  }
}

void GizmoSystem::Shutdown()
{
  for (auto& pair : _objectTypeToRenderer)
  {
    delete pair.second;
  }
  _objectTypeToRenderer.clear();
}
