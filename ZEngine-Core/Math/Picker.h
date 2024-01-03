#pragma once

#include <glm/glm.hpp>
#include <vector>

class AABB;
class Camera;
class Ray;

class Picker 
{
public:
  const AABB* PickAABB(const std::vector<AABB>& aabbs, const Ray& ray);
  bool RayIntersectsAABB(const Ray& ray, const AABB& box, float& intersectionDistance);
  Ray GetRayFromScreenPos(int mouseX, int mouseY, const Camera* camera);

private:
  glm::vec2 GetNDC(int mouseX, int mouseY, const Camera* camera);
};
