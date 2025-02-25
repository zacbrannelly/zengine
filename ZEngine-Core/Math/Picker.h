#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace ZEngine
{
  class AABB;
  class Camera;
  class Ray;

  class Picker 
  {
  public:
    const AABB* PickAABB(const std::vector<AABB>& aabbs, const Ray& ray);
    bool RayIntersectsAABB(const Ray& ray, const AABB& box, float& intersectionDistance);
  };
}
