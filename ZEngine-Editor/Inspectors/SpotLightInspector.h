#pragma once

#include "Inspector.h"
#include <glm/glm.hpp>

namespace ZEngine
{
  class SpotLightInspector : public Inspector
  {
  public:
    SpotLightInspector();
    ~SpotLightInspector();

    void RenderElement() override;
  };
}
