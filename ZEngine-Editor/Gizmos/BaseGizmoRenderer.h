#pragma once
#include <vector>

class Component;

class BaseGizmoRenderer
{
public:
  BaseGizmoRenderer() = default;
  virtual ~BaseGizmoRenderer() = default;

  virtual void Render(int viewId, const std::vector<Component*>& components) = 0;
};
