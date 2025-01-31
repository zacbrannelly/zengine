#pragma once
#include <vector>

namespace ZEngine
{
  class Component;

  class BaseGizmoRenderer
  {
  public:
    BaseGizmoRenderer() = default;
    virtual ~BaseGizmoRenderer() = default;

    virtual void Render(int viewId, const std::vector<Component*>& components) = 0;
  };
}
