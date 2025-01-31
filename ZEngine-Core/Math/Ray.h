#pragma once

#include <glm/glm.hpp>

namespace ZEngine
{
  class Ray
  {
  public:
    glm::vec3 origin;
    glm::vec3 direction;

    Ray(const glm::vec3& origin, const glm::vec3& direction) 
      : origin(origin), direction(glm::normalize(direction)) {}

    static Ray FromScreenPos(int mouseX, int mouseY, const class Camera* camera);
    static glm::vec2 GetNDC(int mouseX, int mouseY, const class Camera* camera);
  };
}
