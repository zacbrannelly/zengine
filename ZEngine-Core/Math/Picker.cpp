#include "Picker.h"
#include "Ray.h"
#include "AABB.h"
#include "../Component/Camera.h"
#include "../Map/Objects/Entity.h"
#include "../Component/Transform.h"

const AABB* Picker::PickAABB(const std::vector<AABB>& aabbs, const Ray& ray)
{
  const AABB* pickedBox = nullptr;
  float minDistance = FLT_MAX;

  for (const auto& box : aabbs)
  {
    float distance;
    if (RayIntersectsAABB(ray, box, distance) && distance < minDistance)
    {
      minDistance = distance;
      pickedBox = &box;
    }
  }

  return pickedBox;
}

bool Picker::RayIntersectsAABB(const Ray& ray, const AABB& box, float& intersectionDistance)
{
  float tMin = 0.0f; // set to -FLT_MAX for negative values
  float tMax = FLT_MAX;

  const glm::vec3 boxBounds[2] = {box.GetMin(), box.GetMax()};

  for (int i = 0; i < 3; ++i) { // For each axis
    if (std::abs(ray.direction[i]) < std::numeric_limits<float>::epsilon())
    {
      // Ray is parallel to slab. No hit if origin not within slab
      if (ray.origin[i] < boxBounds[0][i] || ray.origin[i] > boxBounds[1][i]) 
        return false;
    }
    else
    {
      // Compute intersection t value of ray with near and far plane of slab
      float ood = 1.0f / ray.direction[i];
      float t1 = (boxBounds[0][i] - ray.origin[i]) * ood;
      float t2 = (boxBounds[1][i] - ray.origin[i]) * ood;

      // Make t1 be intersection with near plane, t2 with far plane
      if (t1 > t2) std::swap(t1, t2);

      // Compute the intersection of slab intersection intervals
      tMin = std::max(tMin, t1);
      tMax = std::min(tMax, t2);

      // Exit with no collision as soon as slab intersection becomes empty
      if (tMin > tMax) return false;
    }
  }

  // Ray intersects all 3 slabs. Return point (tMin) of first intersection
  intersectionDistance = tMin;
  return true;
}
