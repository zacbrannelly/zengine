#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
  glm::vec3 origin;
  glm::vec3 direction;

  Ray(const glm::vec3& origin, const glm::vec3& direction) 
    : origin(origin), direction(glm::normalize(direction)) {}
};
