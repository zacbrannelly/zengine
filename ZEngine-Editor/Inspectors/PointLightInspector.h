#pragma once

#include "Inspector.h"
#include <glm/glm.hpp>

namespace ZEngine
{
  class PointLightInspector : public Inspector
  {
  public:
    PointLightInspector();
    ~PointLightInspector();

    void RenderElement() override;
  };
}
