#pragma once

#include <ZEngine-Core/ImmediateUI/Container.h>
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>

class Editor;
class MapView;

class MapViewToolbar : public Container
{
public:
  MapViewToolbar(Editor* editor, MapView* mapView);
  ~MapViewToolbar();

  ImGuizmo::OPERATION GetObjectGizmoOp() const;
  ImGuizmo::MODE GetObjectGizmoMode() const;
    
  void RenderElement() override;

private:
  Editor* _editor;
  MapView* _mapView;
  ImGuizmo::OPERATION _objectGizmoOp;
  ImGuizmo::MODE _objectGizmoMode;
};
